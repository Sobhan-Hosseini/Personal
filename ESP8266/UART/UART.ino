/*
  UART1
  Author: Seyed Sobhan Hosseini
  Date: ‎Wednesday, ‎July ‎26, ‎2023, ‏‎11:48:21 AM
*/

#include <Arduino.h>

int colVal = 0;
int potNumber = 0;
int counter = 1;
bool state = false;
const int interruptPin = D2;
const int potPin = A0;
void IRAM_ATTR change();

void setup(){
  Serial.begin(9600);
  pinMode(potPin, INPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), change, FALLING);
}

void loop(){
  if(state){
    if(counter == 3){
      counter = 1;
    }else{
      counter = counter + 1;
    }
    state = false;
  }

  potNumber = analogRead(potPin);
  colVal = map(potNumber, 0, 1024, 0, 255);

  String strcounter = String(counter);
  String strcolVal = String(colVal);

  Serial.write(counter);
  Serial.write(colVal);

  delay(1000);
}

void IRAM_ATTR change(){
  state = true;
}
