/*
  Joystick
  Author: Seyed Sobhan Hosseini
  Date: Thursday, July 27, 2023, 7:48:22 PM
*/

#include <Arduino.h>

int joypin = A0;
int buzpin = D1;
int joyVal = 0;
int pwm = 0;

void setup(){
  Serial.begin(115200);
  pinMode(joypin, INPUT);
  pinMode(buzpin, OUTPUT);
}

void loop(){
  joyVal = analogRead(joypin);
  pwm = map(joyVal, 0, 1024, 0, 255);
  analogWrite(buzpin, pwm);
  Serial.println(pwm);
}
