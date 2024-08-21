/*
  Creating Tasks
  Author: Seyed Sobhan Hosseini
  Date: ‎‎Saturday, ‎August ‎12, ‎2023, ‏‎7:25:14 PM
*/

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

static const int led = LED_BUILTIN;

void LED(void *parameter){
  while(true){
    digitalWrite(led, HIGH);
    vTaskDelay(600 / portTICK_PERIOD_MS);
    digitalWrite(led, LOW);
    vTaskDelay(600 / portTICK_PERIOD_MS); 
  }
}

void LED2(void *parameter){
  while(true){
    digitalWrite(led, HIGH);
    vTaskDelay(400 / portTICK_PERIOD_MS);
    digitalWrite(led, LOW);
    vTaskDelay(400 / portTICK_PERIOD_MS); 
  }
}

void setup(){
  pinMode(led, OUTPUT);

  xTaskCreatePinnedToCore(
    LED,
    "first delay",
    1024,
    NULL,
    1,
    NULL,
    app_cpu);

   xTaskCreatePinnedToCore(
    LED2,
    "second delay",
    1024,
    NULL,
    1,
    NULL,
    app_cpu);

}

void loop(){

}
