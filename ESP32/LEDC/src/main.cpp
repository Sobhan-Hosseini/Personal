/*
  LEDC
  Author: Seyed Sobhan Hosseini
  Date: Wednesday, November 29, 2023, 6:18:57 PM
*/

#include <Arduino.h>

const int ledPin = 2;
const int freq = 5000;
const int channel = 0;
const int resulotion = 8;
int dutyCycle = 0;

void setup(){
  ledcSetup(channel, freq, resulotion);
  ledcAttachPin(ledPin, channel);
}

void loop(){
  if(dutyCycle < 256){
    ledcWrite(channel, dutyCycle);
  }else{
    dutyCycle = 0;
  }
  dutyCycle++;
  Serial.println(dutyCycle);
  delay(100)
}
