/*
  Binary Counter with LED Display
  Author: Seyed Sobhan Hosseini
  Date: Sunday, July 16, 2023, 2:52:26 PM
*/

#include <Arduino.h>

int counter = 0;
int pin;

void setup(){
  Serial.begin(115200);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);

}

void loop(){
  if(digitalRead(D5) == HIGH){
   counter = counter + 1;
  }else{
    counter = counter - 1;
  }
    for(int i = 3; i >= 0; i--){
        if(i == 3){
          pin = D1;
        }else if(i == 2){
          pin = D2;
        }else if(i == 1){
          pin = D6;
        }else{
          pin = D7;
        }
        int k = counter >> i;
        if(k & 1)
            digitalWrite(pin, HIGH);
        else
            digitalWrite(pin, LOW);
    }
    delay(1000);
}
