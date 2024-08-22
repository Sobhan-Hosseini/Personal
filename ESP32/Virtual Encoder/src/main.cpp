/*
  Virtual Encoder
  Author: Seyed Sobhan Hosseini
  Date: Monday, December 4, 2023, 10:06:00 PM
*/

#include <Arduino.h>
#include <TMCStepper.h>
#include "driver/uart.h"
#include "driver/timer.h"

#define SERIAL_PORT Serial1
#define DRIVER_ADDRESS 0b00
#define R_SENSE 0.11f
#define TIMER_SCALE (APB_CLK_FREQ / 128000)

TMC2209Stepper driver(&SERIAL_PORT , R_SENSE , DRIVER_ADDRESS);
uart_config_t uart_config = {
    .baud_rate = 115200,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
};
timer_config_t timer;

int v = 5;
int delta_x = 50;
int delta_time = 0;
int gain = 0;
int vactual = v * gain;

void setup(){
    uart_set_pin(UART_NUM_2, 25, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_param_config(UART_NUM_2, &uart_config);
    uart_driver_install(UART_NUM_2, 1024, 1536, 0, 0, 0);

    driver.begin();
    driver.en_spreadCycle(false);
    driver.toff(4);
    driver.blank_time(10);
    driver.TCOOLTHRS(0xFFFFF);
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
    driver.freewheel(0);

    timer.divider = 128;
    timer.counter_dir = TIMER_COUNT_UP;
    timer.counter_en = TIMER_START;
    timer.alarm_en = TIMER_ALARM_EN;
    timer.auto_reload = TIMER_AUTORELOAD_EN;
    timer_init(TIMER_GROUP_0, TIMER_0, &timer);
    timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0);
    timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, 20 * TIMER_SCALE);
    timer_enable_intr(TIMER_GROUP_0, TIMER_0);
    timer_pause(TIMER_GROUP_0, TIMER_0);
    // timer_isr_register(TIMER_GROUP_0, TIMER_0, distance_timer_isr, nullptr, ESP_INTR_FLAG_IRAM, nullptr);
    delta_time = delta_x / v;
    timer_start(TIMER_GROUP_0, TIMER_0);
}

void loop(){
    if(timer.timer_counter_value < delta_time){
      driver.VACTUAL(vactual);
    }else{
      driver.VACTUAL(0);
    }
}
