/*
  Sim800l
  Author: Seyed Sobhan Hosseini
  Date: Monday, October 16, 2023, 3:45:49 PM
*/

#include <Arduino.h>

void updateSerial(){
  delay(500);
  while(Serial.available()){
    Serial2.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(Serial2.available()){
    Serial.write(Serial2.read());//Forward what Software Serial received to Serial Port
  }
}

void test_sim800_module(){
  Serial2.println("AT");
  updateSerial();
  Serial.println();
  Serial2.println("AT+CSQ");
  updateSerial();
  Serial2.println("AT+CCID");
  updateSerial();
  Serial2.println("AT+CREG?");
  updateSerial();
  Serial2.println("ATI");
  updateSerial();
  Serial2.println("AT+CBC");
  updateSerial();
}

void send_SMS(){
  Serial2.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  Serial2.println("AT+CMGS=\"+989216503788\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  Serial2.print("Enipha !"); //text content
  updateSerial();
  Serial.println();
  Serial.println("Message Sent");
  Serial2.write(26);
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  delay(3000);
  test_sim800_module();
  send_SMS();
}

void loop() {
  updateSerial();
}
