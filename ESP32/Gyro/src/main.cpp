/*
  Gyro
  Author: Seyed Sobhan Hosseini
  Date: Tuesday, August 8, 2023, 7:03:49 PM
*/

#include <Arduino.h>
#include "Wire.h"
 
const int gy_addr=0x68; // I2C address 
 
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
byte check_I2c(byte addr);

void setup(){
Wire.begin();
Serial.begin(115200);
 
Wire.beginTransmission(gy_addr);
Wire.write(0x6B); // PWR_MGMT_1 register
Wire.write(0);
Wire.endTransmission(true);
}
 
void loop(){
  Wire.beginTransmission(gy_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);

  Wire.requestFrom(gy_addr,14,true);

  AcX=Wire.read()<<8|Wire.read();
  AcY=Wire.read()<<8|Wire.read();
  AcZ=Wire.read()<<8|Wire.read();
  Tmp=Wire.read()<<8|Wire.read();
  GyX=Wire.read()<<8|Wire.read();
  GyY=Wire.read()<<8|Wire.read();
  GyZ=Wire.read()<<8|Wire.read();

  Serial.print("AcX = "); Serial.print(AcX);
  Serial.print(" | AcY = "); Serial.print(AcY);
  Serial.print(" | AcZ = "); Serial.print(AcZ);
  Serial.print(" | Tmp = "); Serial.print(Tmp);
  Serial.print(" | GyX = "); Serial.print(GyX);
  Serial.print(" | GyY = "); Serial.print(GyY);
  Serial.print(" | GyZ = "); Serial.println(GyZ);
  
  delay(500);
}
 
