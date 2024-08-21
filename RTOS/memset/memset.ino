/*
  memset
  Author: Seyed Sobhan Hosseini
  Date: ‎‎Saturday, ‎August ‎12, ‎2023, ‏‎8:20:21 PM
*/

#include <stdlib.h>

#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

static const int led_pin = LED_BUILTIN;
static int led_delay = 500;

void LED(void *parameter){
  while (true){
    digitalWrite(led_pin, HIGH);
    vTaskDelay(led_delay / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(led_delay / portTICK_PERIOD_MS);
  }
}

void setDelay(void *parameters){
  char a;
  char b[5]; //buffer
  uint8_t x = 0;
  
  memset(b, 0,5);

  while (true){
    if (Serial.available() > 0){
      a = Serial.read();

      if (a == '\n'){
        led_delay = atoi(b);
        Serial.print("the new LED delay is : ");
        Serial.println(led_delay);
        memset(b, 0, 5);
        x = 0;
      }else{
        if (x < 4){
          b[x] = a;
          x++;
        }
      }
    }
  }
}

void setup(){
  pinMode(led_pin, OUTPUT);
  
  Serial.begin(115200);

    xTaskCreatePinnedToCore(  
            LED,    
            "blink",  
            1024,           
            NULL,           
            1,              
            NULL,           
            app_cpu);

    xTaskCreatePinnedToCore(  
              setDelay,     
              "Set delay",  
              1024,           
              NULL,           
              1,              
              NULL,           
              app_cpu);

    vTaskDelete(NULL);
}

void loop(){

}
