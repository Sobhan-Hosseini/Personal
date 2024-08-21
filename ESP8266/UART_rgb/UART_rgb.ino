/*
  UART2
  Author: Seyed Sobhan Hosseini
  Date: ‎Wednesday, ‎July ‎26, ‎2023, ‏‎11:48:43 AM
*/

#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial uart(2, 3); // RX, TX pins

const int red = D4;
const int green = D6;
const int blue = D7;

void setup(){
  Serial.begin(9600);
  uart.begin(9600);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
}

void loop(){
  Serial.println(Serial.available());
  if (Serial.available()>0){
    Serial.println("in");
    String strcolor = Serial.readUntil('\n');
    String strvalue = Serial.readStringUntil('\n');

    int color = strcolor.toInt();
    int value = strvalue.toInt();

    if(color == 1){
      analogWrite(red, value);
      Serial.println(color);
      Serial.println(value);
    }else if(color == 2){
      analogWrite(green, value);
      Serial.println(color);
      Serial.println(value);
    }else if(color == 3){
      analogWrite(green, value);
      Serial.println(color);
      Serial.println(value);
    }
  }
}
