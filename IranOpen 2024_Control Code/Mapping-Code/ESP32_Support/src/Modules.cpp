#include "Modules.h"

Modules modules;
Compass cmps;
Uart uart;

static QueueHandle_t control_rxqueue;
QueueHandle_t control_txqueue, debug_txqueue;
SemaphoreHandle_t cmps_data_mutex, mpu_data_mutex;
TaskHandle_t i2c_task_handler;

Adafruit_NeoPixel battery(8, BATTERY_LIGHT, NEO_GRB + NEO_KHZ800);
char telnet_array[2][BUFF_LEN] = {0};
char package[101];
char control_array[6][BUFF_LEN] = {0};
char debug_array[6][BUFF_LEN] = {0};
char temp[BUFF_LEN] = {0};
#define ENDLINE "\n"
#define TAB "\t"
unsigned long motor_en_ms = 0;
unsigned long pre_motor_en_ms = 0;
bool MOTOR_STATE = 0;
double expsmoothing(double, double, double);
double adc_read_voltage();
void battery_light(void *);
void battery_light_on(int, int, int, int);
void Modules::setup()
{
    gpio_config_t gpio;
    gpio.intr_type = GPIO_INTR_DISABLE;
    gpio.mode = GPIO_MODE_OUTPUT;
    gpio.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio.pin_bit_mask = (1ULL << MUX) | (1ULL << MOTOR_EN_PIN);
    gpio_config(&gpio);
    gpio_set_level(MUX, CONTROL);
    gpio_set_level(MOTOR_EN_PIN, LOW);

    gpio_config_t motor_en_butt;
    
    cmps_data_mutex = xSemaphoreCreateMutex();
    mpu_data_mutex = xSemaphoreCreateMutex();

    uart.config_uart();
    vTaskDelay(1000);

    adc1_config_width(ADC_WIDTH_12Bit);
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
    // pinMode(MOTOR_EN_BUTTON, INPUT_PULLUP);
    // attachInterrupt(MOTOR_EN_BUTTON, motor_en_isr,FALLING);
    xTaskCreatePinnedToCore(
        battery_light,
        "battery neopixel",
        2048,
        NULL,
        13,
        NULL,
        APP_CPU_NUM);
}

void Modules::i2c_bus_setup()
{
    i2c_config_t config;
    config.mode = I2C_MODE_MASTER;
    config.master.clk_speed = I2C_MASTER_FREQ;
    config.scl_io_num = SCL_PIN;
    config.scl_pullup_en = GPIO_PULLUP_ENABLE;
    config.sda_io_num = SDA_PIN;
    config.sda_pullup_en = GPIO_PULLUP_ENABLE;
    config.clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL;
    i2c_param_config(I2C_NUM_0, &config);
    i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);

    cmps.setup();
    mpu.mpu_setup();

    xTaskCreatePinnedToCore(
        i2c_bus_updater,
        "i2c bus updater function",
        2048,
        NULL,
        18,
        &i2c_task_handler,
        PRO_CPU_NUM);
}

void i2c_bus_updater(void *)
{
    while (true)
    {
        if (cmps.CMPS)
        {
            cmps.update();
        }
        vTaskDelay(20);
    }
}

// static void IRAM_ATTR motor_en_isr()
// {
//     motor_en_ms = millis();
//     if(motor_en_ms - pre_motor_en_ms > 250)
//     {
//         MOTOR_STATE = !MOTOR_STATE;
//         gpio_set_level(MOTOR_EN_PIN, MOTOR_STATE);
//         pre_motor_en_ms = motor_en_ms;
//     }
// }

double adc_read_voltage()
{
    double reading = double(adc1_get_raw(ADC1_CHANNEL_6));
    double khiar_adc = 1.02584;
    if (reading < 1 || reading > 4095)
    {
        return 0;
    }
    // return -0.000000000009824 * pow(reading,3) + 0.000000016557283 * pow(reading,2) + 0.000854596860691 * reading + 0.065440348345433;
    reading =  -0.000000000000016 * pow(reading, 4) + 0.000000000118171 * pow(reading, 3) - 0.000000301211691 * pow(reading, 2) + 0.001109019271794 * reading + 0.034143524634089;
    return reading / khiar_adc;
} // Added an improved polynomial, use either, comment out as required

double v3_to_v12(double v_in)
{
    return v_in   * 5.581;
}

void battery_light_on(int light_num, int r, int g, int b)
{
    for(int i = 0;i < light_num; i++)
    {
        battery.setPixelColor(i, battery.Color(r,g,b));
        battery.show();
    }
}

void battery_light(void *)
{
    double read_voltage = 0.0, battery_voltage = 0.0, diff = 0.0;
    battery.begin();
    while (true)
    {
        read_voltage = adc_read_voltage();
        uart.debug_print("battery: ");
        uart.debug_print(read_voltage);
        battery_voltage = v3_to_v12(read_voltage);
        uart.debug_print("\text: ");
        uart.debug_println(battery_voltage);
        // battery_light_on(5,255,255,255);
        for(int i=0; i<8;i++)
        {
            battery.setBrightness(20);
        }
        if((battery_voltage > 12.3) && (battery_voltage <= 12.6))
        {
            battery_light_on(5,0,255,0);
        }
        else if((battery_voltage > 12.1) && (battery_voltage <= 12.3))
        {
            battery_light_on(4,0,255,0);
        }
        else if((battery_voltage > 11.9) && (battery_voltage <= 12.1))
        {
            battery_light_on(3,0,255,0);
        }
        else if((battery_voltage > 11.6) && (battery_voltage <= 11.9))
        {
            battery_light_on(2,0,255,0);
        }
        else if((battery_voltage > 11.4) && (battery_voltage <= 11.6))
        {
            battery_light_on(1,0,255,0);
        }
        else if((battery_voltage > 11.2) && (battery_voltage <= 11.4)) 
        {
            battery_light_on(5, 255, 0, 0);
        }
        else if(battery_voltage <= 11.2)
        {
            battery_light_on(5,255,0,0);
            vTaskDelay(100);
            battery_light_on(5,0,0,0);
            vTaskDelay(100);
        }
        // else{
        //     battery_light_on(5,255,255,255);
        // }
        vTaskDelay(20);
    }
}

void Compass::setup()
{
    if (check())
    {
        erase_calibration();
        disable_calibration();
        enable_calibration();
        vTaskDelay(10);
        cmps_cmd = i2c_cmd_link_create_static(i2c_cmd, I2C_LINK_RECOMMENDED_SIZE(2));
        i2c_master_start(cmps_cmd);
        i2c_master_write_byte(cmps_cmd, (Compass_Address << 1) | I2C_MASTER_WRITE, true);
        i2c_master_write(cmps_cmd, commands, 1, true);
        i2c_master_start(cmps_cmd);
        i2c_master_write_byte(cmps_cmd, (Compass_Address << 1) | I2C_MASTER_READ, true);
        i2c_master_read(cmps_cmd, cmps_result_temp, 2, I2C_MASTER_LAST_NACK);
        i2c_master_stop(cmps_cmd);
    }
}

bool Compass::check()
{
    cmps_cmd = i2c_cmd_link_create_static(i2c_cmd, I2C_LINK_RECOMMENDED_SIZE(2));
    i2c_master_start(cmps_cmd);
    i2c_master_write_byte(cmps_cmd, (Compass_Address << 1) | I2C_MASTER_WRITE, true);
    i2c_master_stop(cmps_cmd);
    for (int i = 0, status = 0; i < 10; i++)
    {
        i2c_master_cmd_begin(I2C_NUM_0, cmps_cmd, 10);
        if (status != 263 && status != -1)
        {
            i2c_cmd_link_delete_static(cmps_cmd);
            return CMPS = true;
        }
        vTaskDelay(100);
    }
    i2c_cmd_link_delete_static(cmps_cmd);
    return CMPS;
}

void Compass::i2c_command(uint8_t *commands)
{
    cmps_cmd = i2c_cmd_link_create_static(i2c_cmd, I2C_LINK_RECOMMENDED_SIZE(2));
    i2c_master_start(cmps_cmd);
    i2c_master_write_byte(cmps_cmd, (Compass_Address << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write(cmps_cmd, commands, 2, true);
    i2c_master_stop(cmps_cmd);
    i2c_master_cmd_begin(I2C_NUM_0, cmps_cmd, 50);
    i2c_cmd_link_delete_static(cmps_cmd);
}

void Compass::erase_calibration()
{
    i2c_command(&commands[1]);
    vTaskDelay(20);
    i2c_command(&commands[3]);
    vTaskDelay(20);
    i2c_command(&commands[5]);
    vTaskDelay(300);
}

void Compass::disable_calibration()
{
    i2c_command(&commands[7]);
    vTaskDelay(20);
    i2c_command(&commands[9]);
    vTaskDelay(20);
    i2c_command(&commands[11]);
    vTaskDelay(20);
    i2c_command(&commands[13]);
    vTaskDelay(20);
}

void Compass::enable_calibration()
{
    i2c_command(&commands[7]);
    vTaskDelay(20);
    i2c_command(&commands[9]);
    vTaskDelay(20);
    i2c_command(&commands[11]);
    vTaskDelay(20);
    i2c_command(&commands[15]);
    vTaskDelay(20);
}

void Compass::update()
{
    if (xSemaphoreTake(cmps_data_mutex, 1) == pdTRUE)
    {
        i2c_master_cmd_begin(I2C_NUM_0, cmps_cmd, 50);
        if (cmps_result_temp[0] < 0xF)
            cmps_result[0] = (int)(cmps_result_temp[0] << 8 | cmps_result_temp[1]) / 10.0;
        previous_data = cmps_result[0];
        xSemaphoreGive(cmps_data_mutex);
    }
}

double Compass::get_car_angle()
{
    double temp;
    if (xSemaphoreTake(cmps_data_mutex, 1) == pdTRUE)
    {
        temp = cmps_result[0];
        xSemaphoreGive(cmps_data_mutex);
    }
    else
    {
        temp = previous_data;
    }
    return temp;
}

void MPU::mpu_register_read(uint8_t reg_addr, uint8_t *data, size_t len)
{
    i2c_master_write_read_device(I2C_MASTER_NUM, MPU_Sensor_Address, &reg_addr, 1, data, len, 0);
}

void MPU::mpu_register_write_byte(uint8_t reg_addr, uint8_t data)
{
    int ret;
    uint8_t write_buf[2] = {reg_addr, data};
    ret = i2c_master_write_to_device(I2C_MASTER_NUM, MPU_Sensor_Address, write_buf, sizeof(write_buf), 0);
}

bool MPU::mpu_check()
{
    mpu_cmd = i2c_cmd_link_create_static(i2c_cmd, I2C_LINK_RECOMMENDED_SIZE(2));
    i2c_master_start(mpu_cmd);
    i2c_master_write_byte(mpu_cmd, (MPU_Sensor_Address << 1) | I2C_MASTER_WRITE, true);
    i2c_master_stop(mpu_cmd);
    for (int i = 0, status = 0; i < 10; i++)
    {
        status = i2c_master_cmd_begin(I2C_NUM_0, mpu_cmd, 10);
        if (status != 263 && status != -1)
        {
            i2c_cmd_link_delete_static(mpu_cmd);
            return Mpu = true;
        }
        vTaskDelay(100);
    }
    i2c_cmd_link_delete_static(mpu_cmd);
    return Mpu;
}

void MPU::mpu_setup()
{
    mpu_clear_reg();
    mpu_set_sensitivity();
    uart.debug_print("3.mpu setup: ");
    while (mpu_check() == 0)
    {
        uart.debug_print('.');
        delayMicroseconds(10);
    }
    uart.debug_println(" done");
}

void MPU::mpu_clear_reg()
{
    mpu_register_write_byte(MPU_PWR_MGMT_1, 0);
    vTaskDelay(10);
    mpu_register_write_byte(MPU_REG_CONFIG, 0x1);
    vTaskDelay(10);
}

void MPU::mpu_set_sensitivity()
{
    mpu_register_write_byte(MPU_REG_GYR_CONFIG, 0x2);
    vTaskDelay(10);
    mpu_register_write_byte(MPU_REG_ACCEL_CONFIG, 0x2);
    vTaskDelay(10);
}

double MPU::mpu_get_yaw()
{
    double temp = expsmoothing(alpha, (double(mpu_result[5])) / MPU_GYR_SCALE, first_previous_data) + 0.85;
    gyro_Z = expsmoothing(beta, temp, second_previous_data);
    first_previous_data = (double(mpu_result[5])) / MPU_GYR_SCALE;
    second_previous_data = temp;
    // uart.debug_print("gyro z: ");
    // uart.debug_println(gyro_Z);
    if ((gyro_Z > -3.5) && (gyro_Z < 3.5))
    {
        gyro_Z = 0.0;
    }
    else // if(abs(gyro_Z) >= 3.5)
    {
        double k_yaw = 0.0;
        if (gyro_Z > 0)
        {
            k_yaw = 2.149 * 0.9;
        }
        else if (gyro_Z < 0)
        {
            k_yaw = 2.921 * 7.1;
        }
        yaw += k_yaw * gyro_Z * double(1.0 / MPU_GYRO_FREQ);
    }
    if (yaw < 0)
    {
        yaw += 360.0;
    }
    if (yaw > 360)
    {
        yaw -= 360;
    }
    return yaw;
}

double expsmoothing(double coeff, double next, double pre)
{
    return coeff * next + (1 - coeff) * pre;
}

void debug_tx(void *)
{
    char *buffer = (char *)malloc(BUFF_LEN);
    size_t length;
    while (true)
    {
        if (xQueueReceive(debug_txqueue, (void *)buffer, 1) == pdTRUE)
        {
            for (int i = 0; i < BUFF_LEN; i++)
            {
                if (buffer[i] == '\0')
                {
                    length = i;
                    break;
                }
                length = i + 1;
            }
            uart_write_bytes(UART_NUM_0, (char *)buffer, length);
            bzero(buffer, BUFF_LEN);
        }
        vTaskDelay(5);
    }
}
void Uart::config_uart()
{
    control_txqueue = xQueueCreate(40, 127);
    control_rxqueue = xQueueCreate(40, 127);
    debug_txqueue = xQueueCreate(40, 127);
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
    uart_set_pin(UART_NUM_0, 1, 3, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_param_config(UART_NUM_0, &uart_config);
    uart_driver_install(UART_NUM_0, 1536, 1536, 40, &debug_txqueue, 0);
    xTaskCreatePinnedToCore(
        debug_tx,
        "debug transmitter",
        2560,
        NULL,
        2,
        NULL,
        PRO_CPU_NUM);
}

void Uart::control_print(const char *input)
{
    xQueueSend(control_txqueue, (void *)input, 1);
}
void Uart::control_print(int input)
{
    bzero(&control_array[0][0], BUFF_LEN);
    snprintf(&control_array[0][0], BUFF_LEN, "%d", input);
    xQueueSend(control_txqueue, (void *)&control_array[0][0], 1);
}
void Uart::control_print(double input)
{
    bzero(&control_array[1][0], BUFF_LEN);
    snprintf(&control_array[1][0], BUFF_LEN, "%.2f", input);
    xQueueSend(control_txqueue, (void *)&control_array[1][0], 1);
}
void Uart::control_println(const char *input)
{
    for (int i = 0; i < BUFF_LEN - 1; i++)
    {
        if (input[i] != '\0')
            temp[i] = input[i];
        else
        {
            temp[i] = '\n';
            temp[i] = '\0';
            break;
        }
    }
    temp[126] = '\0';
    xQueueSend(control_txqueue, (void *)temp, 1);
}
void Uart::control_println(int input)
{
    bzero(&control_array[2][0], BUFF_LEN);
    snprintf(&control_array[2][0], BUFF_LEN, "%d\n", input);
    xQueueSend(control_txqueue, (void *)&control_array[2][0], 1);
}
void Uart::control_println(double input)
{
    bzero(&control_array[3][0], BUFF_LEN);
    snprintf(&control_array[3][0], BUFF_LEN, "%.2f\n", input);
    xQueueSend(control_txqueue, (void *)&control_array[3][0], 1);
}
void Uart::control_printtab(const char *input)
{
    for (int i = 0; i < BUFF_LEN - 1; i++)
    {
        if (input[i] != '\0')
            temp[i] = input[i];
        else
        {
            temp[i] = '\t';
            temp[i] = '\0';
            break;
        }
    }
    temp[126] = '\0';
    xQueueSend(control_txqueue, (void *)temp, 1);
}
void Uart::control_printtab(int input)
{
    bzero(&control_array[4][0], BUFF_LEN);
    snprintf(&control_array[4][0], BUFF_LEN, "%d\t", input);
    xQueueSend(control_txqueue, (void *)&control_array[4][0], 1);
}
void Uart::control_printtab(double input)
{
    bzero(&control_array[5][0], BUFF_LEN);
    snprintf(&control_array[5][0], BUFF_LEN, "%.2f\t", input);
    xQueueSend(control_txqueue, (void *)&control_array[5][0], 1);
}
void Uart::debug_print(const char *input)
{
    xQueueSend(debug_txqueue, (void *)input, 1);
}
void Uart::debug_print(int input)
{
    bzero(&debug_array[0][0], BUFF_LEN);
    snprintf(&debug_array[0][0], BUFF_LEN, "%d", input);
    xQueueSend(debug_txqueue, (void *)&debug_array[0][0], 1);
}
void Uart::debug_print(double input)
{
    bzero(&debug_array[1][0], BUFF_LEN);
    snprintf(&debug_array[1][0], BUFF_LEN, "%.2f", input);
    xQueueSend(debug_txqueue, (void *)&debug_array[1][0], 1);
}
void Uart::debug_println(const char *input)
{
    xQueueSend(debug_txqueue, (void *)input, 1);
    xQueueSend(debug_txqueue, (void *)ENDLINE, 1);
}
void Uart::debug_println(int input)
{
    bzero(&debug_array[2][0], BUFF_LEN);
    snprintf(&debug_array[2][0], BUFF_LEN, "%d\n", input);
    xQueueSend(debug_txqueue, (void *)&debug_array[2][0], 1);
}
void Uart::debug_println(double input)
{
    bzero(&debug_array[3][0], BUFF_LEN);
    snprintf(&debug_array[3][0], BUFF_LEN, "%.2f\n", input);
    xQueueSend(debug_txqueue, (void *)&debug_array[3][0], 1);
}
void Uart::debug_printtab(const char *input)
{
    xQueueSend(debug_txqueue, (void *)input, 1);
    xQueueSend(debug_txqueue, (void *)TAB, 1);
}
void Uart::debug_printtab(int input)
{
    bzero(&debug_array[4][0], BUFF_LEN);
    snprintf(&debug_array[4][0], BUFF_LEN, "%d\t", input);
    xQueueSend(debug_txqueue, (void *)&debug_array[4][0], 1);
}
void Uart::debug_printtab(double input)
{
    bzero(&debug_array[5][0], BUFF_LEN);
    snprintf(&debug_array[5][0], BUFF_LEN, "%.2f\t", input);
    xQueueSend(debug_txqueue, (void *)&debug_array[5][0], 1);
}
void Uart::collection(const char *format, ...)
{
    for (int i = 0; i < 101; i++)
        package[i] = '\0';
    va_list list;
    va_start(list, format);
    for (int i = 0; i < 10; i++)
    {
        if (*format == '\0')
        {
            package[i * 10 - 1] = '\n';
            break;
        }
        if (*format == 'd')
        {
            snprintf(&package[i * 10], 10, "%d        ", va_arg(list, int));
            package[i * 10 + 9] = ' ';
        }
        else if (*format == 'f')
        {
            snprintf(&package[i * 10], 10, "%.3f        ", va_arg(list, double));
            package[i * 10 + 9] = ' ';
        }
        format++;
    }
    package[99] = '\n';
    package[100] = '\0';
    va_end(list);
    uart.debug_print(package);
}