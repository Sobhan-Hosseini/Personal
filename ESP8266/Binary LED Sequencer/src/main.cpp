/*
  Binary LED Sequencer
  Author: Seyed Sobhan Hosseini
  Date: Wednesday, July 19, 2023, 10:18:24 AM
*/

#include <Arduino.h>

int counter = 255;
int pin;
int a[8];
int b[3];
int n = 0;

void setup(){
  Serial.begin(115200);
  pinMode(D0 , OUTPUT);
  pinMode(D1 , OUTPUT);
  pinMode(D2 , OUTPUT);
    for(int i = 7; i >= 0; i--){
      int k = counter >> i;
      if(k & 1)
          a[i] = 1;
      else
          a[i] = 0;
  }
}

void loop(){
  for(int j = 0; j < 8; j++){
    if(a[j] == 1){
      for(int i = 2; i >= 0; i--){
        int h = j >> i;
        if (h & 1)
            b[i] = 1;
        else
            b[i] = 0;
        }
        digitalWrite(D0, b[0]);
        digitalWrite(D1, b[1]);
        digitalWrite(D2, b[2]);
    }
      delay(1);
  }
}
