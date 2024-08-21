/*
  Single vl
  Author: Seyed Sobhan Hosseini
  Date: Wednesday, July 19, 2023, 10:25:49 AM
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
  delay(10);
}

void loop(){
  int dis = distanceSensor.getDistance();
  Serial.println(dis);
  delay(200);
}
