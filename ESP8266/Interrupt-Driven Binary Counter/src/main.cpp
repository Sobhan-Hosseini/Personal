/*
  Interrupt-Driven Binary Counter
  Author: Seyed Sobhan Hosseini
  Date: Wednesday, July 19, 2023, 10:18:23 AM
*/

#include <Arduino.h>

int counter = 0;
int pin;
bool state = false;
void IRAM_ATTR PUSH();

void setup(){
  Serial.begin(115200);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(14), PUSH, FALLING);
}

void loop(){
    if(state == true){
   counter = counter - 1;
  }else{
    counter = counter + 1;
  }
    for (int i = 3; i >= 0; i--) {
        if(i == 3){
          pin = D1;
        }else if(i == 2){
          pin = D2;
        }else if(i == 1){
          pin = D3;
        }else{
          pin = D4;
        }
        int k = counter >> i;
        if (k & 1)
            digitalWrite(pin, HIGH);
        else
            digitalWrite(pin, LOW);
    }
    delay(1000);
}

void PUSH(){
  state = !state;
}
