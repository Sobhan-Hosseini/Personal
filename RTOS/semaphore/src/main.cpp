/*
  Semaphore
  Author: Seyed Sobhan Hosseini
  Date: Monday, July 22, 2024, 8:59:55 PM
*/

#include <Arduino.h>s
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <esp_sleep.h>

// Create a semaphore handle
SemaphoreHandle_t core1Semaphore;

// Deep sleep parameters
#define SLEEP_TIME_S 10  // Time in seconds for deep sleep

void core1Task(void *pvParameters) {
    while (true) {
        // Wait for the semaphore to be available
        if (xSemaphoreTake(core1Semaphore, portMAX_DELAY) == pdTRUE) {
            // Task is active, perform operations here
            Serial.println("Core 1 Task is running");
            delay(1000); // Simulate work
            xSemaphoreGive(core1Semaphore); // Release the semaphore to allow core 0 to suspend this task
            vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay to allow other tasks to run
        }
    }
}

void core0Task(void *pvParameters) {
    while (true) {
        // Perform deep sleep
        Serial.println("Core 0 Task is going to sleep");
        
        // Set up deep sleep
        esp_sleep_enable_timer_wakeup(SLEEP_TIME_S * 1000000);  // Convert seconds to microseconds
        
        // Deep sleep
        esp_deep_sleep_start();

        // After waking up, print message
        Serial.println("Core 0 Task has woken up");

        // Control the core1 task
        Serial.println("Core 0 Task is controlling core 1");
        xSemaphoreGive(core1Semaphore); // Activate core1 task
        vTaskDelay(5000 / portTICK_PERIOD_MS); // Run core1 task for 5 seconds
        xSemaphoreTake(core1Semaphore, portMAX_DELAY); // Deactivate core1 task
        Serial.println("Core 1 Task is suspended");
        vTaskDelay(5000 / portTICK_PERIOD_MS); // Wait for 5 seconds before reactivating
    }
}

void setup(){
    Serial.begin(115200);
    delay(1000); // Allow serial to initialize

    // Create the semaphore
    core1Semaphore = xSemaphoreCreateBinary();

    // Create tasks pinned to specific cores
    xTaskCreatePinnedToCore(core1Task, "Core 1 Task", 10000, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(core0Task, "Core 0 Task", 10000, NULL, 1, NULL, 0);
}

void loop(){
    // Empty loop as we are using FreeRTOS tasks
}
