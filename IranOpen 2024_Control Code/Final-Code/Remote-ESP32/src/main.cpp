/*
  IranOpen 2024 Final Av Remote Code
  Authors: Kian Khatibi
  Date: Spring 2024
*/

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <string.h>
// #include <SPI.h>
#include "driver/uart.h"
#include "driver/mcpwm.h"
#include "TMCStepper.h"
#include "Adafruit_NeoPixel.h"

#define REAR_LIGHT 13
#define HEAD_LIGHT 23
#define SERVO_OUT 16
#define SERVO_MAX_RIGHT 100
#define SERVO_MAX_LEFT -100
#define SPEEDSTEP 10
#define DRIVER_ADDRESS 0b00
#define BUFF_LEN 127
#define TMC_Serial_Port Serial2

Adafruit_NeoPixel rear(27, REAR_LIGHT, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel head(28, HEAD_LIGHT, NEO_GRB + NEO_KHZ800);
TMC2209Stepper driver(NULL, 0.11f, DRIVER_ADDRESS);
mcpwm_config_t servo;
QueueHandle_t debug_txqueue;

#define ENDLINE "\n\0"
#define TAB "\t\0"
char debug_array[6][BUFF_LEN] = {0};

typedef struct struct_message
{
  bool remote;
  int speed_val, steer_val;
} struct_message;

struct_message rcv_data;
int current_speed = 0, input_speed = 0, input_steer = 0, current_steer = 0;
char C;
bool get_val = 0;
void rcv_callback(const uint8_t *, const uint8_t *, int);
void remote_lights_on();
void remote_lights_off();
void config_uart();
void motor_setup();
void steer_setup();
void turn(int);
void UART_accelerate(int speed);
void operate(int _steer, int _speed);
// void debug_tx(void *);
// void debug_print(const char *input);
// void debug_print(int input);
// void debug_print(double input);
// void debug_println(const char *input);
// void debug_println(int input);
// void debug_println(double input);
// void debug_printtab(const char *input);
// void debug_printtab(int input);
// void debug_printtab(double input);

void setup()
{
  Serial.begin(115200);
  delay(1000);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  delay(1000);
  digitalWrite(2, LOW);
  config_uart();
  motor_setup();
  steer_setup();
  // TMC_Serial_Port.begin(115200);
  delay(1000);
  // Serial.println("HEllooooooooooooooooooo");
  WiFi.mode(WIFI_STA);
  // Serial.print("MAC Address: ");
  // Serial.println(WiFi.macAddress());
  // debug_print("Initializing ESP-NOW ");
  Serial.print("Initializing ESP-NOW ");
  while (esp_now_init() != ESP_OK)
  {
    // debug_print(".");
    Serial.print(".");
    delay(1);
  }
  // debug_println(' ');
  Serial.println("done");
  esp_now_register_recv_cb(rcv_callback);
  rear.begin();
  head.begin();
  operate(0,0);
  
  delay(1000);
}

void loop()
{
  if (rcv_data.remote == 1)
  {
    remote_lights_on();
    // debug_print("control: ");
    // debug_printtab(rcv_data.control);
    // debug_print("Speed: ");
    // debug_printtab(rcv_data.speed_val);
    // debug_print("Steer: ");
    // debug_printtab(rcv_data.steer_val);
    // debug_println(' ');
    if (get_val == 1)
    {
      input_speed = rcv_data.speed_val;
      input_steer = rcv_data.steer_val;
      get_val = 0;
    }
  }
  else
  {
    remote_lights_off();
    input_speed = 0;
    input_steer = 0;
  }
  // input_steer += 3;
  // if(input_steer > 100)
  // {
  //   input_steer = 0;
  // }
  // UART_accelerate(input_speed);
  // // UART_accelerate(20);
  // turn(input_steer);
  // operate(input_steer, input_speed);
  operate(50, 50);

  // Serial.print("  rcv Speed: ");
  // Serial.print(rcv_data.speed_val);
  // Serial.print("  rcv Steer: ");
  // Serial.print(rcv_data.steer_val);
  Serial.print("  input speed: ");
  Serial.print(input_speed);
  Serial.print("  input steer: ");
  Serial.print(input_steer);
  // delay(500);
}

void rcv_callback(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  memcpy(&rcv_data, incomingData, sizeof(rcv_data));
  get_val = 1;
}

void remote_lights_on()
{
  for (int i = 10; i < 17; i++)
  {
    rear.setBrightness(50);
    rear.setPixelColor(i, rear.Color(255, 255, 255));
    rear.show();
  }

  for (int i = 10; i < 16; i++)
  {
    head.setBrightness(50);
    head.setPixelColor(i, head.Color(255, 255, 255));
    head.show();
  }
}

void remote_lights_off()
{
  for (int i = 11; i < 16; i++)
  {
    rear.setBrightness(0);
    rear.setPixelColor(i, rear.Color(0, 0, 0));
    rear.show();
  }

  for (int i = 10; i < 17; i++)
  {
    head.setBrightness(0);
    head.setPixelColor(i, head.Color(0, 0, 0));
    head.show();
  }
}

void motor_setup()
{
  driver.begin();
  driver.en_spreadCycle(false);
  driver.toff(4);
  driver.blank_time(10);
  driver.TCOOLTHRS(0xFFFFF); // 20bit max
  driver.semin(5);
  driver.semax(2);
  driver.shaft(true);
  driver.SGTHRS(2);
  driver.irun(17);
  driver.ihold(0);
  driver.vsense(false);
  driver.iholddelay(2);
  driver.TPOWERDOWN(12);
  driver.VACTUAL(0);
  driver.freewheel(1);
  driver.microsteps(64);
}

void UART_accelerate(int speed)
{
  Serial.println("\t\tin accel");
  if (speed < -100)
  {
    speed = -100;
  }
  else if (speed > 100)
  {
    speed = 100;
  }

  if (speed != current_speed)
  {
    if (current_speed == 0)
    {
      driver.freewheel(0);
    }
    while (speed - current_speed > SPEEDSTEP)
    {
      current_speed += SPEEDSTEP;
      if (current_speed == 0)
      {
        driver.freewheel(0);
      }
      if (current_speed == 60)
      {
        vTaskDelay(50);
      }
      driver.VACTUAL(map(current_speed, -100, 100, -100000, 100000));
      if ((current_speed > 20) || (current_speed < -20))
      {
        driver.en_spreadCycle(true);
      }
      else
      {
        driver.en_spreadCycle(false);
      }
      // debug_print("current speed: ");
      // debug_println(current_speed);
      // Serial.print("current speed: ");
      // Serial.print(current_speed);
      delay(2);
    }
    while (speed - current_speed < -SPEEDSTEP)
    {
      current_speed -= SPEEDSTEP;
      if (current_speed == 0)
      {
        driver.freewheel(0);
      }
      if (current_speed == -60)
        // {
        vTaskDelay(50);
    }
    driver.VACTUAL(map(current_speed, -100, 100, -100000, 100000));
    if ((current_speed > 20) || (current_speed < -20))
    {
      driver.en_spreadCycle(true);
    }
    else
    {
      driver.en_spreadCycle(false);
    }
  }
  if (speed == 0)
  {
    driver.freewheel(1);
  }
  if (speed > 20 || speed < -20)
  {
    driver.en_spreadCycle(true);
  }
  else
  {
    driver.en_spreadCycle(false);
  }
  // debug_print("current speed: ");
  // debug_println(current_speed);
  // Serial.print("current speed: ");
  // Serial.print(current_speed);
  current_speed = speed;
}

void config_uart()
{
  // debug_txqueue = xQueueCreate(40, BUFF_LEN);
  uart_config_t uart_config = {
      .baud_rate = 115200,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
  };
  // uart_set_pin(UART_NUM_0, 1, 3, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
  // uart_param_config(UART_NUM_0, &uart_config);
  // uart_driver_install(UART_NUM_0, 1536, 1536, 0, NULL, 0);
  uart_set_pin(UART_NUM_2, 25, 14, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
  uart_param_config(UART_NUM_2, &uart_config);
  uart_driver_install(UART_NUM_2, 1536, 1536, 0, NULL, 0);

  // xTaskCreatePinnedToCore(
  //     debug_tx,
  //     "debug transmitter",
  //     2560,
  //     NULL,
  //     2,
  //     NULL,
  //     APP_CPU_NUM);
}
/*
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

void debug_print(const char *input)
{
  xQueueSend(debug_txqueue, (void *)input, 1);
}

void debug_print(int input)
{
  bzero(&debug_array[0][0], BUFF_LEN);
  snprintf(&debug_array[0][0], BUFF_LEN, "%d", input);
  xQueueSend(debug_txqueue, (void *)&debug_array[0][0], 1);
}

void debug_print(double input)
{
  bzero(&debug_array[1][0], BUFF_LEN);
  snprintf(&debug_array[1][0], BUFF_LEN, "%.2f", input);
  xQueueSend(debug_txqueue, (void *)&debug_array[1][0], 1);
}

void debug_println(const char *input)
{
  xQueueSend(debug_txqueue, (void *)input, 1);
  xQueueSend(debug_txqueue, (void *)ENDLINE, 1);
}

void debug_println(int input)
{
  bzero(&debug_array[2][0], BUFF_LEN);
  snprintf(&debug_array[2][0], BUFF_LEN, "%d\n", input);
  xQueueSend(debug_txqueue, (void *)&debug_array[2][0], 1);
}

void debug_println(double input)
{
  bzero(&debug_array[3][0], BUFF_LEN);
  snprintf(&debug_array[3][0], BUFF_LEN, "%.2f\n", input);
  xQueueSend(debug_txqueue, (void *)&debug_array[3][0], 1);
}

void debug_printtab(const char *input)
{
  xQueueSend(debug_txqueue, (void *)input, 1);
  xQueueSend(debug_txqueue, (void *)TAB, 1);
}

void debug_printtab(int input)
{
  bzero(&debug_array[4][0], BUFF_LEN);
  snprintf(&debug_array[4][0], BUFF_LEN, "%d\t", input);
  xQueueSend(debug_txqueue, (void *)&debug_array[4][0], 1);
}

void debug_printtab(double input)
{
  bzero(&debug_array[5][0], BUFF_LEN);
  snprintf(&debug_array[5][0], BUFF_LEN, "%.2f\t", input);
  xQueueSend(debug_txqueue, (void *)&debug_array[5][0], 1);
}
*/
void steer_setup()
{
  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, SERVO_OUT);
  servo.frequency = 50;
  servo.cmpr_a = 5.43;
  servo.duty_mode = MCPWM_DUTY_MODE_0;
  servo.counter_mode = MCPWM_UP_COUNTER;
  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &servo);
  Serial.println("\n4.Servo motor setup done.");
}

void turn(int steer)
{
  if (steer > SERVO_MAX_RIGHT)
  {
    steer = SERVO_MAX_RIGHT;
  }
  else if (steer < SERVO_MAX_LEFT)
  {
    steer = SERVO_MAX_LEFT;
  }
  current_steer = steer;
  servo.cmpr_a = map(-steer, -100, +100, 3.3, 7.56); // right + ... 3.15,7.71
  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &servo);
}

void operate(int _steer, int _speed)
{
    if (_steer > 100)
    {
        _steer = 100;
    }
    else if (_steer < -100)
    {
        _steer = -100;
    }
    else
    {
        _steer = _steer;
    }

    if (_speed > 100)
    {
        _speed = 100;
    }
    else if (_speed < -100)
    {
        _speed = -100;
    }

    turn(_steer);
    UART_accelerate(_speed);
}
