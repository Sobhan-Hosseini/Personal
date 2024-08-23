/*
  ESP32 Random Generator
  Authors: Seyed Sobhan Hosseini - Kian Khatibi
  Date: Thursday, June 20, 2024, 1:49:24 PM
*/

#include <Arduino.h>
#include <HardwareSerial.h>

HardwareSerial SerialPort(2);

void setup(){
  Serial.begin(115200);
  delay(1000);
  SerialPort.begin(115200, SERIAL_8N1, 16, 17);
}

void loop(){
  int random_num = rand() % 256;

  Serial.println(random_num);
  
  SerialPort.write(random_num);
  
  delay(2000);
}
