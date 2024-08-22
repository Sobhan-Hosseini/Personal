/*
  MCPWM
  Author: Seyed Sobhan Hosseini
  Date: Thursday, November 30, 2023, 3:34:22 PM
*/

#include <Arduino.h>
#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"

mcpwm_config_t servo;

const int servo_gpio = 16;
int steer = 0;

void setup() {
   mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, servo_gpio);
    servo.frequency = 50;
    servo.cmpr_a = 5.72;
    servo.duty_mode = MCPWM_DUTY_MODE_0;
    servo.counter_mode = MCPWM_UP_COUNTER;
}

void loop() {
  servo.cmpr_a = 3.4;
  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &servo);
  delay(1000);
  servo.cmpr_a = 5.4;
  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &servo);
  delay(1000);
  servo.cmpr_a = 6.4;
  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &servo);
  delay(1000);
  servo.cmpr_a = 7;
  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &servo);
  delay(1000);
}
