/*
  TMC2209
  Author: Seyed Sobhan Hosseini
  Date: Thursday, August 22, 2024, 2:05:20 PM
*/

#include <Arduino.h>
#include <TMCStepper.h>
#include "driver/uart.h"

#define SERIAL_PORT Serial1
#define DRIVER_ADDRESS 0b00
#define R_SENSE 0.11f

TMC2209Stepper driver(&SERIAL_PORT , R_SENSE , DRIVER_ADDRESS);
uart_config_t uart_config = {
    .baud_rate = 115200,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
};

int speed = 0;

void setup(){
    Serial.begin(115200);

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
}

void loop(){
    if(speed > 500000){
        // driver.freewheel(1);
        speed = 0;
    }else{
       driver.VACTUAL(speed); 
    }
    delay(1);
    speed++;
    Serial.println(speed);
}
