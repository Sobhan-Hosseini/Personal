/*
  UART
  Author: Seyed Sobhan Hosseini
  Date: Thursday, July 27, 2023, 6:09:43 PM
*/

#include <Arduino.h>

int colVal = 0;
int potNumber = 0;
int counter = 1;
bool state = false;
const int interruptPin = D1;
const int potPin = A0;
void IRAM_ATTR change();
char a[7];
char intTochar(int n){
  if(n == 0)return '0';
  if(n == 1)return '1';
  if(n == 2)return '2';
  if(n == 3)return '3';
  if(n == 4)return '4';
  if(n == 5)return '5';
  if(n == 6)return '6';
  if(n == 7)return '7';
  if(n == 8)return '8';
  if(n == 9)return '9';
  return 'f';
}

void setup(){
  Serial.begin(115200);
  pinMode(potPin, INPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), change, FALLING);
}

void loop(){
  a[0] = 'c';

  if(state){
    if(counter == 3){
      counter = 1;
      a[1] = intTochar(counter);
    }else{
      counter = counter + 1;
      a[1] = intTochar(counter);
    }
    state = false;
  }

  a[2] = 'v';
  potNumber = analogRead(potPin);
  colVal = map(potNumber, 0, 1024, 0, 255);

  if(colVal > -1 && colVal < 10){
    a[3] = intTochar(0);
    a[4] = intTochar(0);
    a[5] = intTochar(colVal);
  }else if(colVal > 9 && colVal <100 ){
    a[3] = intTochar(0);
    a[4] = intTochar(colVal / 10);
    a[5] = intTochar(colVal % 10);
  }else if(colVal > 99 && colVal <256){
    a[3] = intTochar(colVal / 100);
    a[4] = intTochar((colVal / 10) % 10);
    a[5] = intTochar(colVal % 10);
  }
  a[6] = '\n';
  
  for(int i = 0; i < 7; i++){
    Serial.print(a[i]);
  }
  delay(1000);
}

void IRAM_ATTR change(){
  state = !state;
}
