/*
  RFID
  Author: Seyed Sobhan Hosseini
  Date: Wednesday, August 9, 2023, 7:19:53 PM
*/

#include <SPI.h>
#include <MFRC522.h>

constexpr uint8_t RST_PIN = D3; 
constexpr uint8_t SS_PIN = D4;

MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

String tag;

void setup(){
  Serial.begin(115200);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
  pinMode(D8, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
}

void loop(){
  if( ! rfid.PICC_IsNewCardPresent())
    return;
  if(rfid.PICC_ReadCardSerial()){
    for(byte i = 0; i < 4; i++){
      tag += rfid.uid.uidByte[i];
    }
    Serial.println(tag);
    if(tag == "891211263"){
        digitalWrite(D1, HIGH);
        delay(100);
    }else{
        digitalWrite(D2, HIGH);
        delay(100);
    }
    tag = "";
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
}
