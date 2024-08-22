/*
  VL53L1X
  Author: Seyed Sobhan Hosseini
  Date: Wednesday, July 19, 2023, 12:07:01 PM
*/

#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_VL53L1X.h"

SFEVL53L1X distanceSensor;

int dis = 0;

void setup(){
  Wire.begin();
  Serial.begin(115200);
  distanceSensor.begin();
  distanceSensor.startRanging();
  distanceSensor.setROI(2,2,199);
  distanceSensor.setDistanceModeLong();
  delay(10);
  
}

void loop(){
  int dis = distanceSensor.getDistance();
  Serial.println(dis);
  delay(200);
}
