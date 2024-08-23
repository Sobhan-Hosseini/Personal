/*
  IranOpen 2024 Final Av Support Code
  Authors: Kian Khatibi - Seyed Sobhan Hosseini - Amirhossein Gholizadeh
  Date: Spring 2024
*/

#include "Modules.h"
#include "Actuator.h"

void System_Init(void *);
void Admininstration(void*);

void setup()
{
    xTaskCreatePinnedToCore(
        System_Init,
        "system initialization",
        4096,
        NULL,
        10,
        NULL,
        APP_CPU_NUM);
    vTaskDelete(0);
}

void System_Init(void*){
    modules.setup();
    motor.setup();
    steer.setup();
    modules.set_controller(CONTROL);
    // xTaskCreatePinnedToCore(
    //     Admininstration,
    //     "Admininstrating the system",
    //     4096,
    //     NULL,
    //     14,
    //     NULL,
    //     APP_CPU_NUM);
    vTaskDelete(0);
}

void Admininstration(void*)
{
    // uart.debug_println("\nAdmin");
    while(1)
    {
        // uart.debug_println("hello");
        vTaskDelay(1);
    }
}

void loop()
{
    vTaskDelete(NULL);
}
