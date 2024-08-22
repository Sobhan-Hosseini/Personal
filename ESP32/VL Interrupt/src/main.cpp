/*
  VL Intrrupt
  Author: Seyed Sobhan Hosseini
  Date: Sunday, July 30, 2023, 11:52:17 AM
*/

#include <Arduino.h>

int dis = 0;

void IRAM_ATTR increase();
void IRAM_ATTR decrease();
void IRAM_ATTR change();

bool incflag = false;
bool decflag = false;
bool changeflag = false;

int scale = 5;
int destination = 0;

void setup() {
  // Wire.begin();
  Serial.begin(115200);
  // vl.begin();
  // vl.startRanging();
  // vl.setROI(2,2,199);
  attachInterrupt(digitalPinToInterrupt(D5), increase, FALLING);
  attachInterrupt(digitalPinToInterrupt(D6), decrease, FALLING);
  attachInterrupt(digitalPinToInterrupt(D7), change, FALLING);
}

void loop() {
  // dis = vl.getDistance();
  if(changeflag == true){
    if(scale == 1){
      scale = 5;
    }
    if(scale == 5){
      scale = 1;
    }
    changeflag = false;
  }

  if(incflag == true){
    destination = destination + scale;
    incflag = false;
  }

  if(decflag == true){
    destination = destination - scale;
    decflag = false;
  }

Serial.println(destination);


}

void IRAM_ATTR increase(){
  incflag = true;
}

void IRAM_ATTR dec(){
  decflag = true;
}

void IRAM_ATTR change(){
  changeflag = true;
}
