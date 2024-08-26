/*
  Dual vl
  Author: Seyed Sobhan Hosseini
  Date: Wednesday, July 19, 2023, 10:18:25 AM
*/

#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_VL53L1X.h"
 
// #define SHUTDOWN_PIN 4
// #define INTERRUPT_PIN 5

SFEVL53L1X vl1;

void setup(void){
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);

  Wire.begin();
  Serial.begin(115200);
  vl1.startRanging();
}

void loop(){
  // vl1.begin();
  digitalWrite(D5, LOW);
  digitalWrite(D6, LOW);
  digitalWrite(D7, LOW);
  
  int distance1 = vl1.getDistance();
  Serial.println(" Distance1 : ");
  Serial.println(distance1);
  digitalWrite(D5, HIGH);
  digitalWrite(D6, LOW);
  digitalWrite(D7, LOW);
  
  int distance2 = vl1.getDistance();
  Serial.println(" Distance2 : ");
  Serial.println(distance2);
  delay(500);
}
