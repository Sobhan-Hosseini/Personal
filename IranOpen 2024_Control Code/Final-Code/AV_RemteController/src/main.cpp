/*
  IranOpen 2024 Final Av Remote Code
  Authors: Kian Khatibi
  Date: Spring 2024
*/

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <string.h>

// MAC Address: A8:42:E3:A9:E5:6C -> Transmitter
// MAC Address: 70:B8:F6:85:D6:34-> Main Receiver(Control ESP) 
//              C8:F0:9E:E7:32:40 -> Support Receiver(Control ESP)
// static uint8_t broadcastAddress[6] = {0x70, 0xB8, 0xF6, 0x85, 0xD6, 0x34};
static uint8_t broadcastAddress[6] = {0xC8,0xF0,0x9E,0xE7,0x32,0x40};

typedef struct struct_message
{
  bool remote;
  int speed_val, steer_val;
} struct_message;

struct_message myData;

esp_now_peer_info_t peerInfo;
char input;
bool speed_change = 1, steer_change = 1;
int speed = 0, steer = 0;
String input_str = "";
esp_err_t result;
bool send = 0, stop = 0;
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup()
{
  Serial.begin(115200);
  delay(1000);
  myData.remote = 1;
  myData.speed_val = 0;
  myData.steer_val = 0;
  WiFi.mode(WIFI_STA);

  Serial.print("Initializing ESP-NOW ");
  while (esp_now_init() != ESP_OK)
  {
    Serial.print('.');
    delay(1);
  }
  Serial.println(' ');
  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer");
    return;
  }
  result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));

  if (result == ESP_OK)
  {
    Serial.println("Sent with success");
  }
  else
  {
    Serial.println("Error sending the data");
  }
  delay(100);
}

void loop()
{
  while (!(Serial.available() > 0))
  {
    delayMicroseconds(1);
  }
  if (Serial.available() > 0)
  {
    input = Serial.read();
    delay(100);
  }

  if ((input == 'w') || (input == 'W'))
  {
    myData.speed_val += 10;
    send = 1;
  }

  else if ((input == 'a') || (input == 'A'))
  {
    // myData.speed_val -= 10;
    myData.steer_val += 10;
    send = 1;
  }
  else if ((input == 's') || (input == 'S'))
  {
    myData.speed_val -= 10;
    // myData.steer_val = 0;
    send = 1;
  }
  else if ((input == 'd') || (input == 'D'))
  {
    // myData.speed_val -= 10;
    myData.steer_val -= 10;
    send = 1;
  }
  else if ((input == 'h') || (input == 'H'))
  {
    myData.speed_val = 0;
    myData.steer_val = 0;
    send = 1;
    stop = 1;
  }
  else if ((input == 'o') || (input == 'O'))
  {
    myData.speed_val = 0;
    myData.steer_val = 0;
    send = 1;
  }

  Serial.print("myData: ");
  Serial.print(myData.remote);
  Serial.print(' ');
  Serial.print(myData.speed_val);
  Serial.print(' ');
  Serial.println(myData.steer_val);

  if (myData.remote == 1 && send == 1)
  {
    if (stop == 'o')
    {
      myData.remote = 0;
    }
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));

    if (result == ESP_OK)
    {
      Serial.println("Sent with success");
    }
    else
    {
      Serial.println("Error sending the data");
    }
    send = 0;
    delay(100);
  }
}
