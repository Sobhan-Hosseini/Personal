/*
  Servo
  Author: Seyed Sobhan Hosseini
  Date: Sunday, July 23, 2023, 8:29:50 PM
*/

#include <Arduino.h>
#include <servo.h>

int position = 0;
int servoPin = D1;
int interruptPin1 = D2;
int interruptPin2 = D3;
bool state = false;
bool flag1 = false;
bool flag2 = false;
Servo servo;

void IRAM_ATTR mosbat();
void IRAM_ATTR manfi();

void setup(){
  Serial.begin(115200);
  servo.attach(servoPin);
  pinMode(interruptPin1, INPUT_PULLUP);
  pinMode(interruptPin2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin1), mosbat, FALLING);
  attachInterrupt(digitalPinToInterrupt(interruptPin2), manfi, FALLING);
}

void loop(){
  if(state == true && flag1 == true){
    Serial.println(state);
      if(position == 180){
        position = 180;
      }else{
        position = position + 10;
      }
      servo.write(position);
      flag1 = false;
      delay(10);
    }

if(state == false && flag2 == true){
      Serial.println(state);
      if(position == 0){
        position = 0;
      }else{
        position = position - 10;
      }
      servo.write(position);
      flag2 = false;
      delay(10);
  }
}

void IRAM_ATTR mosbat(){
  // delay(200);
  state = true;
  flag1 = true;
}

void IRAM_ATTR manfi(){
  // delay(200);
  state = false;
  flag2 = true;
}
