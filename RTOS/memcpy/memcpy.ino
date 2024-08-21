/*
  memcpy
  Author: Seyed Sobhan Hosseini
  Date: ‎Wednesday, ‎August ‎23, ‎2023, ‏‎8:19:41 PM
*/

#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

static char *msg_ptr = NULL;
static volatile uint8_t flag = 0;

void readSerial(void *parameters){
  char a;
  char b[255];
  uint8_t x = 0;

  memset(b, 0, 255);

  while (true){
    if (Serial.available() > 0){
      a = Serial.read();

      if (x < 254){
        b[x] = a;
        x++;
      }
      
      if (a == '\n'){
        b[x - 1] = '\0';

        if (flag == 0){
          msg_ptr = (char *)pvPortMalloc(x * sizeof(char));

          configASSERT(msg_ptr);

          memcpy(msg_ptr, b, x);
          
          flag = 1;
        }
        memset(b, 0, 255);
        x = 0;
      }
    }
  }
}

void printMessage(void *parameters){
  while (true){
    if (flag == 1) {
      Serial.println(msg_ptr);

      vPortFree(msg_ptr);
      msg_ptr = NULL;
      flag = 0;
    }
  }
}

void setup(){
  Serial.begin(115200);

  xTaskCreatePinnedToCore(readSerial,
                          "read serial",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

  xTaskCreatePinnedToCore(printMessage,
                          "print message",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

  vTaskDelete(NULL);
}

void loop(){

}
