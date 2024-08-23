#include "Modules.h"
#include "Actuator.h"

Modules modules;
Compass cmps;
Senesor_MPU sen_MPU;
VL_Sensor vl;
Uart uart;
Jetson vision;
Support support;

TaskHandle_t i2c_task_handler;
SemaphoreHandle_t cmps_data_mutex, mpu_data_mutex;
static QueueHandle_t support_rxqueue, jetson_rxqueue;
QueueHandle_t jetson_txqueue, support_txqueue, debug_txqueue;


#define ENDLINE "\n\0"
#define TAB "\t\0"
char debug_array[6][BUFF_LEN] = {0};
char jetson_array[6][BUFF_LEN] = {0};
char support_array[6][BUFF_LEN] = {0};
char temp[BUFF_LEN] = {0};
char package[101];

Adafruit_NeoPixel rear(27, REAR_LIGHT, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel head(28, HEAD_LIGHT, NEO_GRB + NEO_KHZ800);


void Lights(void*);

void Modules::module_setup()
{
    gpio_config_t gpio;
    gpio.intr_type = GPIO_INTR_DISABLE;
    gpio.mode = GPIO_MODE_OUTPUT;
    gpio.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio.pin_bit_mask = (1ULL << MUX_A) | (1ULL << MUX_B) | (1ULL << MUX_C) | (1ULL << MUX_D) | (1ULL << MODULES_RESET) | (1ULL << LED_BUILTIN);

    gpio_config(&gpio);
    gpio_set_level(MUX_A, LOW);
    gpio_set_level(MUX_B, LOW);
    gpio_set_level(MUX_C, LOW);
    gpio_set_level(MUX_D, LOW);
    gpio_set_level(MODULES_RESET, HIGH); 

    cmps_data_mutex = xSemaphoreCreateMutex();
    mpu_data_mutex = xSemaphoreCreateMutex();
    
    uart.config_uart();
    vTaskDelay(1000);
    i2c_bus_setup();
    vTaskDelay(1000);
    is_remote = 0;
    
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1A, BUZZER);
    buzzer.cmpr_a = 0;
    buzzer.frequency = 1000;
    buzzer.counter_mode = MCPWM_UP_COUNTER;
    buzzer.duty_mode = MCPWM_DUTY_MODE_0;

    xTaskCreatePinnedToCore(
        Lights,
        "lights task",
        2048,
        NULL,
        13,
        NULL,
        APP_CPU_NUM);
}

void Modules::i2c_bus_setup()
{
    // uart.debug_println("\nI2C Bus Setup: ");
    i2c_config_t config;
    config.mode = I2C_MODE_MASTER;
    config.master.clk_speed = I2C_MASTER_FREQ;
    config.scl_io_num = SCL_PIN;
    config.scl_pullup_en = GPIO_PULLUP_ENABLE;
    config.sda_io_num = SDA_PIN;
    config.sda_pullup_en = GPIO_PULLUP_ENABLE;
    config.clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL;
    i2c_param_config(I2C_NUM_0, &config);
    i2c_driver_install(I2C_NUM_0, config.mode, 0, 0, 0);
    // uart.debug_println("I2c Bus Setup Finished");
    delay(10);
    
    
    vl.VL_setup();
    cmps.cmps_setup();
    sen_MPU.mpu_setup();
    
    uart.debug_println("\n3.I2C Bus Setup done.");
    xTaskCreatePinnedToCore(
        i2c_bus_updater,
        "i2c update function",
        3072,
        NULL,
        18,
        NULL,
        PRO_CPU_NUM);
}



void i2c_bus_updater(void *)
{
    while (true)
    {
        for (int i = 0; i < 12; i++)
        {
            if (cmps.CMPS && (i % 2) == 1)
            {
                cmps.update();
            }
            if (sen_MPU.MPU && (i % 2) == 0)
            {
                sen_MPU.mpu_register_update();
            }
            vTaskDelay(4);
            if (i < 8)
            {
                if (i < 4)
                {
                    if ((vl.chosen_VL >> i) & 1)
                    {
                        vl.update_data(i);
                    }
                    if ((vl.chosen_VL >> (i + 4)) & 1)
                    {
                        vl.update_data(i + 4);
                    }
                }
                else
                {
                    if ((vl.chosen_VL >> i) & 1)
                    {
                        vl.update_data(i);
                    }
                    if ((vl.chosen_VL >> (i - 4)) & 1)
                    {
                        vl.update_data(i - 4);
                    }
                }
            }
            else
            {
                if ((vl.chosen_VL >> i) & 1)
                {
                    vl.update_data(i);
                }
            }
            vTaskDelay(6);
        }
    }
    vTaskDelete(NULL);
}

void Modules::lights_off()
{
    for(int i = 0; i < 26; i++)
    {
        rear.setBrightness(50);
        rear.setPixelColor(i, rear.Color(0, 0, 0));
        rear.show();
    }
    
    for(int i = 0; i < 27; i++)
    {
        head.setBrightness(50);
        head.setPixelColor(i, rear.Color(0, 0, 0));
        head.show();
    }
}

void Modules::rear_lights_on(){
    for(int i = 0; i < 6; i++){
        if(i < 5){
            rear.setBrightness(50);
            rear.setPixelColor(i, rear.Color(200, 0, 0));
            rear.show();
        }
        rear.setBrightness(50);
        rear.setPixelColor(i + 21, rear.Color(200, 0, 0));
        rear.show();
    }
}

void Modules::head_lights_on(){
    for (int i = 0; i < 27; i++){
        head.setBrightness(100);
        head.setPixelColor(i, head.Color(255, 210, 100));
        head.show();
    }
}

void Modules::left_flasher(){
    for (int i = 0; i <= 6; i++)
    {
        if(i < 5){
            rear.setBrightness(50);
            rear.setPixelColor(i, rear.Color(240, 50, 5));
            rear.show();
        }
        head.setBrightness(50);
        head.setPixelColor(i + 20, head.Color(240, 50, 5));
        head.show();    
    }
    vTaskDelay(400);

    for (int i = 0; i <= 6; i++)
    {
        if(i < 5){
            rear.setBrightness(50);
            rear.setPixelColor(i, rear.Color(0, 0, 0));
            rear.show();
        }
        head.setBrightness(50);
        head.setPixelColor(i + 20, head.Color(0, 0, 0));
        head.show();    
    }
    vTaskDelay(400);
}

void Modules::right_flasher(){
    for (int i = 0; i <= 5; i++)
    {
        if(i < 5){
            head.setBrightness(50);
            head.setPixelColor(i, head.Color(240, 50, 5));
            head.show();    
        }
        rear.setBrightness(50);
        rear.setPixelColor(i + 21, rear.Color(240, 50, 5));
        rear.show();
    }
    vTaskDelay(400);

    for (int i = 0; i <= 5; i++)
    {
        if(i < 5){
            head.setBrightness(10);
            head.setPixelColor(i, head.Color(0, 0, 0));
            head.show();    
        }
        rear.setBrightness(50);
        rear.setPixelColor(i + 21, rear.Color(0, 0, 0));
        rear.show();
    }
    vTaskDelay(400);
}

void  Modules::both_flasher(){
    for (int i = 0; i <= 6; i++)
    {
        if(i < 5){
            rear.setBrightness(50);
            rear.setPixelColor(i, rear.Color(240, 50, 5));
            rear.show();
        }
        head.setBrightness(50);
        head.setPixelColor(i + 20, head.Color(240, 50, 5));
        head.show();    
    }
    for (int i = 0; i <= 5; i++)
    {
        if(i < 5){
            head.setBrightness(50);
            head.setPixelColor(i, head.Color(240, 50, 5));
            head.show();    
        }
        rear.setBrightness(50);
        rear.setPixelColor(i + 21, rear.Color(240, 50, 5));
        rear.show();
    }
    vTaskDelay(400);

    for (int i = 0; i <= 6; i++)
    {
        if(i < 5){
            rear.setBrightness(50);
            rear.setPixelColor(i, rear.Color(0, 0, 0));
            rear.show();
        }
        head.setBrightness(50);
        head.setPixelColor(i + 20, head.Color(0, 0, 0));
        head.show();    
    }
    for (int i = 0; i <= 5; i++)
    {
        if(i < 5){
            head.setBrightness(10);
            head.setPixelColor(i, head.Color(0, 0, 0));
            head.show();    
        }
        rear.setBrightness(50);
        rear.setPixelColor(i + 21, rear.Color(0, 0, 0));
        rear.show();
    }
    vTaskDelay(400);
}

void Modules::headlights(){
    for (int i = 0; i < 27; i++)
    {
        head.setBrightness(100);
        head.setPixelColor(i, head.Color(255, 210, 100));
        head.show();
    }
    vTaskDelay(100);
    for (int i = 0; i < 27; i++)
    {
        head.setBrightness(100);
        head.setPixelColor(i, head.Color(0, 0, 0));
        head.show();
    }
    vTaskDelay(100);
    for (int i = 0; i < 27; i++)
    {
        head.setBrightness(100);
        head.setPixelColor(i, head.Color(255, 210, 100));
        head.show();
    }
    vTaskDelay(700);
    for (int i = 0; i < 27; i++)
    {
        head.setBrightness(100);
        head.setPixelColor(i, head.Color(0, 0, 0));
        head.show();
    }

    vTaskDelay(100);
}

void Lights(void *)
{
    rear.begin();
    head.begin();
    // battery.begin();
    int pre_speed = 0;
    while (true)
    {
        switch (vision.get_sign())
        {
        case vision.TRAVERSE:
        {
            for(int i = 0; i < 27; i++){   //Front Lights off
                head.setBrightness(50);
                head.setPixelColor(i, head.Color(0, 0, 0));
                head.show();
            }

            if (motor.get_current_speed() < pre_speed)
            {
                for (int i = 5; i <= 20; i++)
                {
                    rear.setBrightness(50);
                    rear.setPixelColor(i, rear.Color(200, 0, 0));
                    rear.show();
                }
                vTaskDelay(500);
            }
            else if (motor.get_current_speed() == 0)
            {
                for (int i = 5; i <= 20; i++)
                {
                    rear.setBrightness(50);
                    rear.setPixelColor(i, rear.Color(200, 0, 0));
                    rear.show();
                }
                vTaskDelay(500);
            }
            else
            {
                for (int i = 5; i <= 20; i++)
                {
                    rear.setBrightness(50);
                    rear.setPixelColor(i, rear.Color(0, 0, 0));
                    rear.show();
                }
            }

            pre_speed = motor.get_current_speed();
            break;
        }   
        
        case vision.STOP:
        {
            if (motor.get_current_speed() < pre_speed)
            {
                for (int i = 5; i <= 20; i++)
                {
                    rear.setBrightness(50);
                    rear.setPixelColor(i, rear.Color(200, 0, 0));
                    rear.show();
                }
                vTaskDelay(500);
            }
            else if (motor.get_current_speed() == 0)
            {
                for (int i = 5; i <= 20; i++)
                {
                    rear.setBrightness(50);
                    rear.setPixelColor(i, rear.Color(200, 0, 0));
                    rear.show();
                }
                vTaskDelay(500);
            }
            else{
                for (int i = 5; i <= 20; i++)
                {
                    rear.setBrightness(50);
                    rear.setPixelColor(i, rear.Color(0, 0, 0));
                    rear.show();
                }
            }
            pre_speed = motor.get_current_speed();
            break;
        }

        case vision.RIGHT:
        {
            for (int i = 0; i <= 5; i++)
            {
                if(i < 5){
                    head.setBrightness(50);
                    head.setPixelColor(i, head.Color(240, 50, 5));
                    head.show();    
                }
                rear.setBrightness(50);
                rear.setPixelColor(i + 21, rear.Color(240, 50, 5));
                rear.show();
            }
            vTaskDelay(400);

            for (int i = 0; i <= 5; i++)
            {
                if(i < 5){
                    head.setBrightness(10);
                    head.setPixelColor(i, head.Color(0, 0, 0));
                    head.show();    
                }
                rear.setBrightness(50);
                rear.setPixelColor(i + 21, rear.Color(0, 0, 0));
                rear.show();
            }
            // vTaskDelay(400);

            //STOP
            if (motor.get_current_speed() < pre_speed)
            {
                for (int i = 5; i <= 20; i++)
                {
                    rear.setBrightness(50);
                    rear.setPixelColor(i, rear.Color(200, 0, 0));
                    rear.show();
                }
                // vTaskDelay(500);
            }
            else if (motor.get_current_speed() == 0)
            {
                for (int i = 5; i <= 20; i++)
                {
                    rear.setBrightness(50);
                    rear.setPixelColor(i, rear.Color(200, 0, 0));
                    rear.show();
                }
                // vTaskDelay(500);
            }
            else{
                for (int i = 5; i <= 20; i++)
                {
                    rear.setBrightness(50);
                    rear.setPixelColor(i, rear.Color(0, 0, 0));
                    rear.show();
                }
            }
            pre_speed = motor.get_current_speed();        
            vTaskDelay(400);
            break;
        }
        
        case vision.LEFT:
        {
            for (int i = 0; i <= 6; i++)
            {
                if(i < 5){
                    rear.setBrightness(50);
                    rear.setPixelColor(i, rear.Color(240, 50, 5));
                    rear.show();
                }
                head.setBrightness(50);
                head.setPixelColor(i + 20, head.Color(240, 50, 5));
                head.show();    
            }
            vTaskDelay(400);

            for (int i = 0; i <= 6; i++)
            {
                if(i < 5){
                    rear.setBrightness(50);
                    rear.setPixelColor(i, rear.Color(0, 0, 0));
                    rear.show();
                }
                head.setBrightness(50);
                head.setPixelColor(i + 20, head.Color(0, 0, 0));
                head.show();    
            }

            //STOP
            if (motor.get_current_speed() < pre_speed)
            {
                for (int i = 5; i <= 20; i++)
                {
                    rear.setBrightness(50);
                    rear.setPixelColor(i, rear.Color(200, 0, 0));
                    rear.show();
                }
                // vTaskDelay(500);
            }
            else if (motor.get_current_speed() == 0)
            {
                for (int i = 5; i <= 20; i++)
                {
                    rear.setBrightness(50);
                    rear.setPixelColor(i, rear.Color(200, 0, 0));
                    rear.show();
                }
                // vTaskDelay(500);
            }
            else{
                for (int i = 5; i <= 20; i++)
                {
                    rear.setBrightness(50);
                    rear.setPixelColor(i, rear.Color(0, 0, 0));
                    rear.show();
                }
            }
            pre_speed = motor.get_current_speed();        
            vTaskDelay(400);
            break;
        }

        case vision.TUNNEL:
        {
            for (int i = 0; i < 27; i++)
            {
                head.setBrightness(100);
                head.setPixelColor(i, head.Color(255, 210, 100));
                head.show();
            }

            if (motor.get_current_speed() < pre_speed)
            {
                for (int i = 5; i <= 20; i++)
                {
                    rear.setBrightness(50);
                    rear.setPixelColor(i, rear.Color(200, 0, 0));
                    rear.show();
                }
                // vTaskDelay(500);
            }
            else if (motor.get_current_speed() == 0)
            {
                for (int i = 5; i <= 20; i++)
                {
                    rear.setBrightness(50);
                    rear.setPixelColor(i, rear.Color(200, 0, 0));
                    rear.show();
                }
                // vTaskDelay(500);
            }
            else{
                for (int i = 5; i <= 20; i++)
                {
                    rear.setBrightness(50);
                    rear.setPixelColor(i, rear.Color(0, 0, 0));
                    rear.show();
                }
            }
            for(int i = 0; i < 6; i++){
                if(i < 5){
                    rear.setBrightness(50);
                    rear.setPixelColor(i, rear.Color(200, 0, 0));
                    rear.show();
                }
                rear.setBrightness(50);
                rear.setPixelColor(i + 21, rear.Color(200, 0, 0));
                rear.show();
            }
            pre_speed = motor.get_current_speed();  
            vTaskDelay(400);
            break;

        case vision.TEST:
            for (int i = 0; i <= 25; i++)
            {
                rear.setBrightness(100);
                rear.setPixelColor(i, rear.Color(200, 0, 0));
                rear.show();
            }
            for (int i = 0; i < 27; i++)
            {
                head.setBrightness(100);
                head.setPixelColor(i, head.Color(255, 210, 100));
                head.show();
            }
        }

        case vision.PARK_LEFT:
        {
            for (int i = 0; i <= 6; i++)
            {
                if(i < 5){
                    rear.setBrightness(50);
                    rear.setPixelColor(i, rear.Color(240, 50, 5));
                    rear.show();
                }
                head.setBrightness(50);
                head.setPixelColor(i + 20, head.Color(240, 50, 5));
                head.show();    
            }
            vTaskDelay(400);

            for (int i = 0; i <= 6; i++)
            {
                if(i < 5){
                    rear.setBrightness(50);
                    rear.setPixelColor(i, rear.Color(0, 0, 0));
                    rear.show();
                }
                head.setBrightness(50);
                head.setPixelColor(i + 20, head.Color(0, 0, 0));
                head.show();    
            }

            //STOP
            if (motor.get_current_speed() < pre_speed)
            {
                for (int i = 5; i <= 20; i++)
                {
                    rear.setBrightness(50);
                    rear.setPixelColor(i, rear.Color(200, 0, 0));
                    rear.show();
                }
                // vTaskDelay(500);
            }
            else if (motor.get_current_speed() == 0)
            {
                for (int i = 5; i <= 20; i++)
                {
                    rear.setBrightness(50);
                    rear.setPixelColor(i, rear.Color(200, 0, 0));
                    rear.show();
                }
                // vTaskDelay(500);
            }
            else{
                for (int i = 5; i <= 20; i++)
                {
                    rear.setBrightness(50);
                    rear.setPixelColor(i, rear.Color(0, 0, 0));
                    rear.show();
                }
            }
            pre_speed = motor.get_current_speed();        
            vTaskDelay(400);
            break;
        }

        case vision.PARK_RIGHT:
        {
             for (int i = 0; i <= 5; i++)
            {
                if(i < 5){
                    head.setBrightness(50);
                    head.setPixelColor(i, head.Color(240, 50, 5));
                    head.show();    
                }
                rear.setBrightness(50);
                rear.setPixelColor(i + 21, rear.Color(240, 50, 5));
                rear.show();
            }
            vTaskDelay(400);

            for (int i = 0; i <= 5; i++)
            {
                if(i < 5){
                    head.setBrightness(10);
                    head.setPixelColor(i, head.Color(0, 0, 0));
                    head.show();    
                }
                rear.setBrightness(50);
                rear.setPixelColor(i + 21, rear.Color(0, 0, 0));
                rear.show();
            }
            // vTaskDelay(400);

            //STOP
            if (motor.get_current_speed() < pre_speed)
            {
                for (int i = 5; i <= 20; i++)
                {
                    rear.setBrightness(50);
                    rear.setPixelColor(i, rear.Color(200, 0, 0));
                    rear.show();
                }
                // vTaskDelay(500);
            }
            else if (motor.get_current_speed() == 0)
            {
                for (int i = 5; i <= 20; i++)
                {
                    rear.setBrightness(50);
                    rear.setPixelColor(i, rear.Color(200, 0, 0));
                    rear.show();
                }
                // vTaskDelay(500);
            }
            else{
                for (int i = 5; i <= 20; i++)
                {
                    rear.setBrightness(50);
                    rear.setPixelColor(i, rear.Color(0, 0, 0));
                    rear.show();
                }
            }
            pre_speed = motor.get_current_speed();        
            vTaskDelay(400);
            break;
        }

        case vision.PARK_RIGHT_EXIT:
        {
            for (int i = 0; i <= 6; i++)
            {
                if(i < 5){
                    rear.setBrightness(50);
                    rear.setPixelColor(i, rear.Color(240, 50, 5));
                    rear.show();
                }
                head.setBrightness(50);
                head.setPixelColor(i + 20, head.Color(240, 50, 5));
                head.show();    
            }
            vTaskDelay(400);

            for (int i = 0; i <= 6; i++)
            {
                if(i < 5){
                    rear.setBrightness(50);
                    rear.setPixelColor(i, rear.Color(0, 0, 0));
                    rear.show();
                }
                head.setBrightness(50);
                head.setPixelColor(i + 20, head.Color(0, 0, 0));
                head.show();    
            }

            //STOP
            if (motor.get_current_speed() < pre_speed)
            {
                for (int i = 5; i <= 20; i++)
                {
                    rear.setBrightness(50);
                    rear.setPixelColor(i, rear.Color(200, 0, 0));
                    rear.show();
                }
                // vTaskDelay(500);
            }
            else if (motor.get_current_speed() == 0)
            {
                for (int i = 5; i <= 20; i++)
                {
                    rear.setBrightness(50);
                    rear.setPixelColor(i, rear.Color(200, 0, 0));
                    rear.show();
                }
                // vTaskDelay(500);
            }
            else{
                for (int i = 5; i <= 20; i++)
                {
                    rear.setBrightness(50);
                    rear.setPixelColor(i, rear.Color(0, 0, 0));
                    rear.show();
                }
            }
            pre_speed = motor.get_current_speed();        
            vTaskDelay(400);
            break;
        }

        case vision.GIVEWAY:
        {
            modules.headlights();
        }

        case vision.BARRED:
        {
            modules.headlights();
        }

        case vision.CROSSWALK:
        {
            modules.headlights();
        }

        case vision.NOPASSING:
        {
            modules.headlights();
        }
        
        case vision.NOPASSINGEND:
        {
            modules.headlights();
        }

//         case vision.UTURN_:
//             lights_off();
//             for (int i = 20; i <= 27; i++)
//             {
//                 head.setBrightness(20);
//                 head.setPixelColor(i, head.Color(200, 100, 0));
//                 head.show();
//                 vTaskDelay(50);
//             }
//             vTaskDelay(200);
//             for (int i = 20; i <= 27; i++)
//             {
//                 head.setBrightness(5);
//                 head.setPixelColor(i, head.Color(0, 0, 0));
//                 head.show();
//             }
//             vTaskDelay(500);
//             break;

//         case vision.PARALLELPARK:
//             lights_off();
//             for (int i = 0; i <= 26; i++)
//             {
//                 rear.setBrightness(5);
//                 rear.setPixelColor(i, rear.Color(0, 0, 0));
//                 rear.show();
//             }
//             for (int i = 7, j = 19; i >= 0, j <= 26; i--, j++)
//             {
//                 head.setBrightness(20);
//                 head.setPixelColor(i, head.Color(200, 100, 0));
//                 head.show();
//                 rear.setBrightness(20);
//                 rear.setPixelColor(j, rear.Color(200, 100, 0));
//                 rear.show();
//                 vTaskDelay(50);
//             }
//             vTaskDelay(200);
//             for (int i = 7, j = 19; i >= 0, j <= 26; i--, j++)
//             {
//                 head.setBrightness(5);
//                 head.setPixelColor(i, head.Color(0, 0, 0));
//                 head.show();
//                 rear.setBrightness(5);
//                 rear.setPixelColor(j, rear.Color(0, 0, 0));
//                 rear.show();
//             }
//             vTaskDelay(500);
//             break;
//         case vision.EXPARALLELPARK:
//             for (int i = 20, j = 7; i <= 27, j >= 0; i++, j--)
//             {
//                 head.setBrightness(20);
//                 head.setPixelColor(i, head.Color(200, 100, 0));
//                 head.show();
//                 rear.setBrightness(20);
//                 rear.setPixelColor(j, rear.Color(200, 100, 0));
//                 rear.show();
//                 vTaskDelay(50);
//             }
//             // for(int i=7;i>=0;i--)
//             // {
//             //     rear.setBrightness(20);
//             //     rear.setPixelColor(i, rear.Color(200,100,0));
//             //     rear.show();
//             //     vTaskDelay(50);
//             // }
//             vTaskDelay(200);
//             for (int i = 20, j = 7; i <= 27, j >= 0; i++, j--)
//             {
//                 head.setBrightness(5);
//                 head.setPixelColor(i, head.Color(0, 0, 0));
//                 head.show();
//                 rear.setBrightness(5);
//                 rear.setPixelColor(i, rear.Color(0, 0, 0));
//                 rear.show();
//                 vTaskDelay(50);
//             }
//             vTaskDelay(500);
//             break;
//         case vision.CROSSPARK:
//             for (int i = 20; i <= 27; i++)
//             {
//                 head.setBrightness(20);
//                 head.setPixelColor(i, head.Color(200, 100, 0));
//                 head.show();
//                 vTaskDelay(50);
//             }
//             vTaskDelay(200);
//             for (int i = 20; i <= 27; i++)
//             {
//                 head.setBrightness(5);
//                 head.setPixelColor(i, head.Color(0, 0, 0));
//                 head.show();
//             }
//             vTaskDelay(500);
//             break;            

        default:
            for (int i = 0; i < 26; i++)
            {
                rear.setBrightness(0);
                rear.setPixelColor(i, rear.Color(0, 0, 0));
                rear.show();
            }
            for (int i = 0; i < 27; i++)
            {
                head.setBrightness(0);
                head.setPixelColor(i, head.Color(0, 0, 0));
                head.show();
            }
            break;
        }
        vTaskDelay(20);
    }
    vTaskDelete(NULL);
}

void Modules::MUX_controller(int Select)
{
    gpio_set_level(MUX_A, Select & B01);
    gpio_set_level(MUX_B, (Select >> 1) & B01);
    gpio_set_level(MUX_C, (Select >> 2) & B01);
    gpio_set_level(MUX_D, (Select >> 3) & B01);
}

void Modules::reset_modules()
{
    gpio_set_level(MODULES_RESET, LOW);
    vTaskDelay(200);
    gpio_set_level(MODULES_RESET, HIGH);
}

double Modules::expsmoothing(double coeff, double next, double pre)
{
    return coeff * next + (1 - coeff) * pre;
}

void VL_Sensor::VL_setup()
{

    for (int i = 0; i <= 11; i++)
    {
        MUX_controller(i);
        // uart.debug_print("\nvl.");
        // uart.debug_printtab(i);
        delayMicroseconds(1);
        while (vl.begin() != 0)
        {
            uart.debug_print("VL setup: ");
            uart.debug_println(i);
            // uart.debug_println("\n!!!   begin failed, trying again  ");
            break;
        }

        if (i == LEFT)
        {
            vl.setROI(4, 16, 199);
            // vl.setDistanceModeLong();
            vTaskDelay(2);
        }
        if(i == RIGHT){
            vl.setROI(4, 16, 192);
        }
        if (i == RIGHT || i == BACK_RIGHT || i == BACK_LEFT || i == BACK_MID_LEFT || i == BACK_MID_RIGHT || i == BACK)
        {
            // vl.setDistanceModeLong();
            vl.setROI(4, 16, 56);
            vTaskDelay(2);
        }
        if (i == FRONT || i == FRONT_MID_LEFT || i == FRONT_MID_RIGHT || i == FRONT_LEFT || i == FRONT_RIGHT)
        {
            vl.setROI(4,16, 56);
            vTaskDelay(2);
        }
        vTaskDelay(2);
        vl.setDistanceModeShort();
        vl.setTimingBudgetInMs(40);
        vl.setIntermeasurementPeriod(50);
        vl.startRanging();
        vTaskDelay(100);


    }
    uart.debug_println("\n1.VL Setup done.");
}

void VL_Sensor::start_vl(int selected_VL)
{
    chosen_VL |= selected_VL;
    // uart.debug_println("hello");
}

void VL_Sensor::update_data(int selected_VL)
{
    int VL_recieved_data;
    MUX_controller(selected_VL);
    VL_recieved_data = vl.getDistance();
    // uart.debug_print("here: ");
    // uart.debug_println(VL_recieved_data);
    vl.clearInterrupt();
    // uart.debug_println(vl.getRangeStatus()) ;
    if (vl.getRangeStatus() == 1 || vl.getRangeStatus() == 2 || VL_recieved_data == 0)
    {
        VL_data[selected_VL] = not_valid;
    }
    else
    {
        VL_data[selected_VL] = VL_recieved_data;
    }
}

void VL_Sensor::stop_vl(int selected_VL)
{
    chosen_VL = ~(~chosen_VL | selected_VL);
    for (int i = 0; i < 12; i++)
    {
        if ((selected_VL >> i) & 1)
        {
            VL_data[i] = not_valid;
        }
    }
}

void VL_Sensor::test_VLs()
{
    // start_vl(VFR | VFMR | VF | VFML | VFL | VL | VR | VBR | VBMR | VB | VBML | VBL);
    // for (int i = 0; i < 12; i++)
    // {
    //     update_data(i);
    // }
    uart.debug_printtab("\nFront Right:");
    uart.debug_print(get_data(FRONT_RIGHT));
    vTaskDelay(10);
    uart.debug_printtab("\nFront Mid Right:");
    uart.debug_print(get_data(FRONT_MID_RIGHT));
    vTaskDelay(10);
    uart.debug_printtab("\nFront:");
    uart.debug_print(get_data(FRONT));
    vTaskDelay(10);
    uart.debug_println(" ");
    uart.debug_printtab("\nFront Mid Left:");
    uart.debug_print(get_data(FRONT_MID_LEFT));
    vTaskDelay(10);
    uart.debug_printtab("\nFront Left:");
    uart.debug_print(get_data(FRONT_LEFT));
    vTaskDelay(10);
    uart.debug_printtab("\nLeft:");
    uart.debug_print(get_data(LEFT));
    vTaskDelay(10);
    uart.debug_println(" ");
    uart.debug_printtab("\nRight:");
    uart.debug_print(get_data(RIGHT));
    vTaskDelay(10);
    uart.debug_printtab("\nBack Right:");
    uart.debug_print(get_data(BACK_RIGHT));
    vTaskDelay(10);
    uart.debug_printtab("\nBack Mid Right:");
    uart.debug_print(get_data(BACK_MID_RIGHT));
    vTaskDelay(10);
    uart.debug_println(" ");
    uart.debug_printtab("\nBack:");
    uart.debug_print(get_data(BACK));
    vTaskDelay(10);
    uart.debug_printtab("\nBack Mid Left:");
    uart.debug_print(get_data(BACK_MID_LEFT));
    vTaskDelay(10);
    uart.debug_printtab("\nBack Left:");
    uart.debug_print(get_data(BACK_LEFT));
}

void Compass::cmps_setup()
{
    uart.debug_println("\nthis is cmps setup");
    MUX_controller(0);
    delayMicroseconds(5);
    if (cmps_check())
    {
        cmps_erase_calibration();
        cmps_disable_calibration();
        cmps_enable_calibration();
        vTaskDelay(10);
        cmps_cmd = i2c_cmd_link_create_static(i2c_cmd, I2C_LINK_RECOMMENDED_SIZE(2));
        i2c_master_start(cmps_cmd);
        i2c_master_write_byte(cmps_cmd, (Compass_Address << 1) | I2C_MASTER_WRITE, true);
        i2c_master_write(cmps_cmd, commands, 1, true);
        i2c_master_start(cmps_cmd);
        i2c_master_write_byte(cmps_cmd, (Compass_Address << 1) | I2C_MASTER_READ, true);
        i2c_master_read(cmps_cmd, cmps_result_temp, 2, I2C_MASTER_LAST_NACK);
        i2c_master_stop(cmps_cmd);
        uart.debug_println("\n2.CMPS Setup Done.");
    }
    else
    {
        uart.debug_println("\n2.CMPS Setup failed!!!");
    }
    
}
    

void Compass::cmps_i2c_command(uint8_t *commands)
{
    cmps_cmd = i2c_cmd_link_create_static(i2c_cmd, I2C_LINK_RECOMMENDED_SIZE(2));
    i2c_master_start(cmps_cmd);
    i2c_master_write_byte(cmps_cmd, (Compass_Address << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write(cmps_cmd, commands, 2, true);
    i2c_master_stop(cmps_cmd);
    i2c_master_cmd_begin(I2C_NUM_0, cmps_cmd, 50);
    i2c_cmd_link_delete_static(cmps_cmd);
}

bool Compass::cmps_check()
{
    cmps_cmd = i2c_cmd_link_create_static(i2c_cmd, I2C_LINK_RECOMMENDED_SIZE(2));
    i2c_master_start(cmps_cmd);
    i2c_master_write_byte(cmps_cmd, (Compass_Address << 1) | I2C_MASTER_WRITE, true);
    i2c_master_stop(cmps_cmd);
    for (int i = 0, status = 0; i < 10; i++)
    {
        status = i2c_master_cmd_begin(I2C_NUM_0, cmps_cmd, 10);
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

void Compass::cmps_erase_calibration()
{
    cmps_i2c_command(&commands[1]);
    vTaskDelay(20);
    cmps_i2c_command(&commands[3]);
    vTaskDelay(20);
    cmps_i2c_command(&commands[5]);
    vTaskDelay(300);
}

void Compass::cmps_disable_calibration()
{
    cmps_i2c_command(&commands[7]);
    vTaskDelay(20);
    cmps_i2c_command(&commands[9]);
    vTaskDelay(20);
    cmps_i2c_command(&commands[11]);
    vTaskDelay(20);
    cmps_i2c_command(&commands[13]);
    vTaskDelay(20);
}

void Compass::cmps_enable_calibration()
{
    cmps_i2c_command(&commands[7]);
    vTaskDelay(20);
    cmps_i2c_command(&commands[9]);
    vTaskDelay(20);
    cmps_i2c_command(&commands[11]);
    vTaskDelay(20);
    cmps_i2c_command(&commands[15]);
    vTaskDelay(20);
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

void Compass::update()
{
    MUX_controller(0);
    delayMicroseconds(5);
    if (xSemaphoreTake(cmps_data_mutex, 1) == pdTRUE)
    {
        i2c_master_cmd_begin(I2C_NUM_0, cmps_cmd, 50);
        if (cmps_result_temp[0] < 0xF)
            cmps_result[0] = (int)(cmps_result_temp[0] << 8 | cmps_result_temp[1]) / 10.0;
        if (previous_data == cmps_result[0]) //&& motor.get_current_speed() > 10 && (steer.get_current_steer() > 5 || steer.get_current_steer() < -5))
        {
            frozen_counter++;
        }
        else
        {
            frozen_counter = 0;
            is_corrupted = false;
        }
        if (frozen_counter == 20)
        {
            is_corrupted = true;
            // reset_modules();
            frozen_counter = 0;
        }
        previous_data = cmps_result[0];
        xSemaphoreGive(cmps_data_mutex);
    }
}

// int Compass::turn_to_angle(int target_angle){ //!!! move to Motion.cpp
//     // update();
//     // if ((target_angle + get_car_angle()) > 360)
//     // {
//     //     target_angle = target_angle - 360;
//     // }
//     // else if ((target_angle + get_car_angle()) < 0)
//     // {
//     //     target_angle = target_angle + 360;
//     // }
//     calculate_remaining_angle(target_angle);
//     calculate_optimized_steer(remaining_angle);
//     // optimized_steer = optimized_steer * (-38 / 180);

//     if (optimized_steer > 55)
//     {
//         return -steer.max_steer;
//     }
//     else if (optimized_steer < -55)
//     {
//         return steer.max_steer;
//     }
//     else
//     {
//         return -optimized_steer;
//     }
// }
void Senesor_MPU::mpu_register_read(uint8_t reg_addr, uint8_t *data, size_t len)
{
  i2c_master_write_read_device(I2C_MASTER_NUM, MPU_Sensor_Address, &reg_addr, 1, data, len, 0);
}

void Senesor_MPU::mpu_register_write_byte(uint8_t reg_addr, uint8_t data)
{
  int ret;
  uint8_t write_buf[2] = {reg_addr, data};
  ret = i2c_master_write_to_device(I2C_MASTER_NUM, MPU_Sensor_Address, write_buf, sizeof(write_buf), 0);
}

bool Senesor_MPU::mpu_check()
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
            return MPU = true;
        }
        vTaskDelay(100);
    }
    i2c_cmd_link_delete_static(mpu_cmd);
    return MPU;
}

void Senesor_MPU::mpu_setup()
{
    mpu_clear_reg();
    mpu_set_sensitivity();
    uart.debug_print("3.mpu setup: ");
    // while(mpu_check() == 0)
    // {
    //     uart.debug_print('.');
    //     delayMicroseconds(10);
    // }
    uart.debug_println(" done");
}

void Senesor_MPU::mpu_i2c_command(uint8_t *command)
{
    mpu_cmd = i2c_cmd_link_create_static(i2c_cmd, I2C_LINK_RECOMMENDED_SIZE(2));
    i2c_master_start(mpu_cmd);
    i2c_master_write_byte(mpu_cmd, (MPU_Sensor_Address << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write(mpu_cmd, command, 2, true);
    i2c_master_stop(mpu_cmd);
    i2c_master_cmd_begin(I2C_NUM_0, mpu_cmd, 50);
    i2c_cmd_link_delete_static(mpu_cmd);
}

void Senesor_MPU::mpu_clear_reg()
{
    mpu_register_write_byte(MPU_PWR_MGMT_1, 0x80);
    vTaskDelay(10);
    mpu_register_write_byte(MPU_REG_CONFIG, 0x0);
    vTaskDelay(10);
}

void Senesor_MPU::mpu_set_sensitivity()
{
    mpu_register_write_byte(MPU_REG_GYR_CONFIG, 0x2);
    vTaskDelay(10);
    mpu_register_write_byte(MPU_REG_ACCEL_CONFIG, 0x2);
    vTaskDelay(10);
}

void Senesor_MPU::mpu_register_update()
{
    if (xSemaphoreTake(mpu_data_mutex, 1) == pdTRUE)
    {
        mpu_register_read(MPU_REG_ACCEL_X_OUT_H, mpu_result_temp, 14);
        // mpu_result[0] = (int16_t)(mpu_result_temp[0] << 8 | mpu_result_temp[1]); // Accel X
        // mpu_result[1] = (int16_t)(mpu_result_temp[2] << 8 | mpu_result_temp[3]); // Accel Y
        // mpu_result[2] = (int16_t)(mpu_result_temp[4] << 8 | mpu_result_temp[5]); // Accel Z
        // mpu_result[3] = (int16_t)(mpu_result_temp[6] << 8 | mpu_result_temp[7]); // Temperature
        // mpu_result[3] = (int16_t)(mpu_result_temp[8] << 8 | mpu_result_temp[9]);   // Gyro X
        // mpu_result[4] = (int16_t)(mpu_result_temp[10] << 8 | mpu_result_temp[11]); // Gyro Y
        mpu_result[5] = (int16_t)(mpu_result_temp[12] << 8 | mpu_result_temp[13]); // Gyro Z

        xSemaphoreGive(mpu_data_mutex);
    }
}

double Senesor_MPU::mpu_get_yaw()
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
    else //if(abs(gyro_Z) >= 3.5)
    {
        double k_yaw = 0.0;
        if(gyro_Z > 0){
            k_yaw = 2.149 * 0.9;
        }
        else if(gyro_Z < 0){
            k_yaw = 2.921 * 7.1;
        }
        yaw += k_yaw * gyro_Z * double(1.0 / MPU_GYRO_FREQ);
    }
    if (yaw < 0)
    {
        yaw += 360.0;
    }
    if(yaw > 360)
    {
        yaw -= 360;
    }
    return yaw;
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
        vTaskDelay(2);
    }
    vTaskDelete(NULL);
}

void support_tx(void *)
{
    char *buffer = (char *)malloc(BUFF_LEN);
    size_t length;
    while (true)
    {
        if (xQueueReceive(support_txqueue, (void *)buffer, 1) == pdTRUE)
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
            uart_write_bytes(UART_NUM_2, (char *)buffer, length);
            bzero(buffer, BUFF_LEN);
        }
        vTaskDelay(5);
    }
    vTaskDelete(NULL);
}

void jetson_tx(void *)
{
    char *buffer = (char *)malloc(BUFF_LEN);
    size_t length;
    while (true)
    {
        
        if (xQueueReceive(jetson_txqueue, (void *)buffer, 1) == pdTRUE)
        {
            // uart.debug_print("here");
            for (int i = 0; i < BUFF_LEN; i++)
            {
                if (buffer[i] == '\0')
                {
                    length = i;
                    break;
                }
                length = i + 1;
            }
            uart_write_bytes(UART_NUM_1, (char *)buffer, length);
            bzero(buffer, BUFF_LEN);
            
        }
        vTaskDelay(25);
    }
    vTaskDelete(NULL);
}

void jetson_rx(void *)
{
    double time = 0;
    uart_event_t event;
    uint8_t *buffer = (uint8_t *)malloc(36);
    char *hel = (char *)&buffer;
    char *position = (char *)&buffer[0];
    size_t length, fail_counter = 0;
    int temp_sign = 0;
    while (true)
    {
        // time = millis();
        // int i = uxQueueMessagesWaiting(jetson_rxqueue);
        // uart.debug_println(i);
        // uart.debug_println("jetson rx");
        if (xQueueReceive(jetson_rxqueue, (void *)&event, (TickType_t)1))
        {
            // uart.debug_println("jetson rx");
            switch (event.type)
            {
            case UART_DATA:{

                bzero(buffer, 36);
                uart_get_buffered_data_len(UART_NUM_1, &length);
                
                uart_read_bytes(UART_NUM_1, buffer, 36, 4);

                if (buffer[0] == 'k' && buffer[35] == 'k')
                {
                    vision.distance_from_midline = atoi(&position[1]);
                    vision.angle = atoi(&position[6]);
                    if(vision.sign == 19 || vision.sign == 9 || vision.sign == 2 || vision.sign == 16 || (vision.sign == 14 && atoi(&position[10]) != 19))
                    {
                        vision.sign = atoi(&position[10]);
                    }
                    if(atoi(&position[10]) == 15 && vision.sign == 14){
                        vision.sign = 15;
                    }
                    // if(vision.sign == 0 && atoi(&position[10]) != 19){   edit : overtake enable 
                    //     vision.sign = atoi(&position[10]);
                    // }
                    vision.distance_from_sign = atoi(&position[13]);
                    vision.traffic_sign = atoi(&position[17]);
                    vision.traffic_distance = atoi(&position[20]);
                    vision.crosswalk_distance = atoi(&position[24]);
                    vision.crosswalk_angle = atoi(&position[28]);
                    vision.line_direction = atoi(&position[32]);
                    vision.side_detection = atoi(&position[34]);
                }
                uart_flush(UART_NUM_1);
             }break;
            // case UART_BREAK:{
            //     uart.debug_println(210);
            //     }break;
            // case UART_BUFFER_FULL:{
            //     uart.debug_println(211);
            //     }break;
            // case UART_FIFO_OVF:{
            //     int ik = uxQueueMessagesWaiting(jetson_rxqueue);
            //     uart.debug_printtab(ik);
            //     uart.debug_println(212);
            //     }break;
            // case UART_FRAME_ERR:{
            //     uart.debug_println(213);
            //     }break;
            // case UART_PARITY_ERR:{
            //     uart.debug_println(214);
            //     }break;
            // case UART_DATA_BREAK:{
            //     uart.debug_println(215);
            //     }break;
            // case UART_PATTERN_DET:{
            //     uart.debug_println(216);
            //     }break;
            // case UART_EVENT_MAX:{
            //     uart.debug_println(217);
            //     }break;
            // default:{
            //     uart.debug_println("no type");
            //     }break;
            }
        }
        vTaskDelay(15);
        // uart.debug_println(1000 / (millis() - time));
    }
    free(buffer);
    buffer = NULL;
    vTaskDelete(NULL);
}

void support_rx(void *)
{
    uart_event_t event;
    uint8_t *buffer = (uint8_t *)malloc(3);
    char *position = (char *)&buffer[0];
    size_t length, fail_counter = 0;
    int temp_sign = 0;
    while (true)
    {
        if (xQueueReceive(support_rxqueue, (void *)&event, (TickType_t)1))
        {
            switch (event.type)
            {
            case UART_DATA:
                bzero(buffer, 3);
                uart_get_buffered_data_len(UART_NUM_2, &length);
                uart_read_bytes(UART_NUM_2, buffer, 3, 4);
                
                if(buffer[0] == 's' && buffer[2] == 's')
                {
                    if(buffer[1] == 'L')
                    {

                    }
                }
                break;
            }
        }
    }
    free(buffer);
    buffer = NULL;
    vTaskDelete(NULL);
}

void Uart::config_uart()
{
    debug_txqueue = xQueueCreate(40, BUFF_LEN);
    jetson_txqueue = xQueueCreate(40, BUFF_LEN);
    support_txqueue = xQueueCreate(40, BUFF_LEN);
    support_rxqueue = xQueueCreate(40, BUFF_LEN);
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
    uart_set_pin(UART_NUM_0, 1, 3, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_param_config(UART_NUM_0, &uart_config);
    uart_driver_install(UART_NUM_0, 1536, 1536, 0, NULL, 0);
    uart_set_pin(UART_NUM_1, 26, 27, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_driver_install(UART_NUM_1, 1536, 1536, 40, &jetson_rxqueue, 0);
    uart_set_pin(UART_NUM_2, 25, 14, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_param_config(UART_NUM_2, &uart_config);
    uart_driver_install(UART_NUM_2, 1536, 1536, 40, &support_rxqueue, 0);

    xTaskCreatePinnedToCore(
        debug_tx,
        "debug transmitter",
        2560,
        NULL,
        2,
        NULL,
        PRO_CPU_NUM);
    xTaskCreatePinnedToCore(
        jetson_tx,
        "jetson transmitter",
        2560,
        NULL,
        12,
        NULL,
        PRO_CPU_NUM);
    xTaskCreatePinnedToCore(
        jetson_rx,
        "jetson receiver",
        2560,
        NULL,
        14,
        NULL,
        PRO_CPU_NUM);
    xTaskCreatePinnedToCore(
        support_tx,
        "support transmitter",
        2560,
        NULL,
        13,
        NULL,
        PRO_CPU_NUM);
    xTaskCreatePinnedToCore(
        support_rx,
        "support receiver",
        2560,
        NULL,
        11,
        NULL,
        PRO_CPU_NUM);
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

void Uart::jetson_print(const char *input)
{
    xQueueSend(jetson_txqueue, (void *)input, 1);
}

void Uart::jetson_print(int input)
{
    bzero(&jetson_array[0][0], BUFF_LEN);
    snprintf(&jetson_array[0][0], BUFF_LEN, "%d", input);
    xQueueSend(jetson_txqueue, (void *)&jetson_array[0][0], 1);
}

void Uart::jetson_print(double input)
{
    bzero(&jetson_array[1][0], BUFF_LEN);
    snprintf(&jetson_array[1][0], BUFF_LEN, "%.2f", input);
    xQueueSend(jetson_txqueue, (void *)&jetson_array[1][0], 1);
}

void Uart::jetson_println(const char *input)
{
    xQueueSend(jetson_txqueue, (void *)input, 1);
    xQueueSend(jetson_txqueue, (void *)ENDLINE, 1);
}

void Uart::jetson_println(int input)
{
    bzero(&jetson_array[2][0], BUFF_LEN);
    snprintf(&jetson_array[2][0], BUFF_LEN, "%d\n", input);
    xQueueSend(jetson_txqueue, (void *)&jetson_array[2][0], 1);
}

void Uart::jetson_println(double input)
{
    bzero(&jetson_array[3][0], BUFF_LEN);
    snprintf(&jetson_array[3][0], BUFF_LEN, "%.2f\n", input);
    xQueueSend(jetson_txqueue, (void *)&jetson_array[3][0], 1);
}

void Uart::jetson_printtab(const char *input)
{
    xQueueSend(jetson_txqueue, (void *)input, 1);
    xQueueSend(jetson_txqueue, (void *)TAB, 1);
}

void Uart::jetson_printtab(int input)
{
    bzero(&jetson_array[4][0], BUFF_LEN);
    snprintf(&jetson_array[4][0], BUFF_LEN, "%d\t", input);
    xQueueSend(jetson_txqueue, (void *)&jetson_array[4][0], 1);
}

void Uart::jetson_printtab(double input)
{
    bzero(&jetson_array[5][0], BUFF_LEN);
    snprintf(&jetson_array[5][0], BUFF_LEN, "%.2f\t", input);
    xQueueSend(jetson_txqueue, (void *)&jetson_array[5][0], 1);
}

void Uart::support_print(const char *input)
{
    xQueueSend(support_txqueue, (void *)input, 1);
}

void Uart::support_print(int input)
{
    bzero(&support_array[0][0], BUFF_LEN);
    snprintf(&support_array[0][0], BUFF_LEN, "%d", input);
    xQueueSend(support_txqueue, (void *)&support_array[0][0], 1);
}

void Uart::support_print(double input)
{
    bzero(&support_array[1][0], BUFF_LEN);
    snprintf(&support_array[1][0], BUFF_LEN, "%.2f", input);
    xQueueSend(support_txqueue, (void *)&support_array[1][0], 1);
}

void Uart::support_println(const char *input)
{
    for (int i = 0; i < BUFF_LEN - 1; i++)
    {
        if(input[i] != '\0')
            temp[i] = input[i];
        else{
            temp[i] = '\n';
            temp[i] = '\0';
            break;
        }
    }
    temp[126] = '\0';
    xQueueSend(support_txqueue, (void *)temp, 1);
}

void Uart::support_println(int input)
{
    bzero(&support_array[2][0], BUFF_LEN);
    snprintf(&support_array[2][0], BUFF_LEN, "%d\n", input);
    xQueueSend(support_txqueue, (void *)&support_array[2][0], 1);
}

void Uart::support_println(double input)
{
    bzero(&support_array[3][0], BUFF_LEN);
    snprintf(&support_array[3][0], BUFF_LEN, "%.2f\n", input);
    xQueueSend(support_txqueue, (void *)&support_array[3][0], 1);
}

void Uart::support_printtab(const char *input)
{
    for (int i = 0; i < BUFF_LEN - 1; i++)
    {
        if(input[i] != '\0')
            temp[i] = input[i];
        else{
            temp[i] = '\t';
            temp[i] = '\0';
            break;
        }
    }
    temp[126] = '\0';
    xQueueSend(support_txqueue, (void *)temp, 1);
}

void Uart::support_printtab(int input)
{
    bzero(&support_array[4][0], BUFF_LEN);
    snprintf(&support_array[4][0], BUFF_LEN, "%d\t", input);
    xQueueSend(support_txqueue, (void *)&support_array[4][0], 1);
}

void Uart::support_printtab(double input)
{
    bzero(&support_array[5][0], BUFF_LEN);
    snprintf(&support_array[5][0], BUFF_LEN, "%.2f\t", input);
    xQueueSend(support_txqueue, (void *)&support_array[5][0], 1);
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
    uart.jetson_print(package);
}