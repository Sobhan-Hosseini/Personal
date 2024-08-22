/*
  Attendance System with RFID 
  Author: Seyed Sobhan Hosseini
  Date: Thursday, June 6, 2024, 6:21:23 PM
*/

#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <TimeLib.h>

// WiFi Setting
const char* ssid = "Redmi 9T";
const char* password = "11111111";

// RFID Setting
#define RST_PIN 22
#define SS_PIN  5

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create instance of MFRC522

// Create server instance
AsyncWebServer server(80);

// User list
struct User {
  String uid;
  String name;
  bool present;
  unsigned long startTime;
  unsigned long totalTime;
};

User users[] = {
  {"5979c3f", "sobhan hosseini", false, 0, 0},
  {"876e43f1", "ghazal ghoreishi", false, 0, 0},
  {"abcdef12", "kian khatibi", false, 0, 0},
  {"87654321", "gholizade", false, 0, 0},
  {"abcdef12", "madani", false, 0, 0}
};

const int numUsers = sizeof(users) / sizeof(users[0]);

String readRFID() {
  // If card is not present, return empty
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return "";
  }
  
  // Convert card UID to string
  String rfid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    rfid += String(mfrc522.uid.uidByte[i], HEX);
  }
  
  // Halt the card
  mfrc522.PICC_HaltA();
  
  return rfid;
}

void addUser(String name, String uid) {
  // Find empty slot in the user array
  int emptyIndex = -1;
  for (int i = 0; i < numUsers; i++) {
    if (users[i].uid == "") {
      emptyIndex = i;
      break;
    }
  }

  // If empty slot found, add the new user
  if (emptyIndex != -1) {
    users[emptyIndex].uid = uid;
    users[emptyIndex].name = name;
    users[emptyIndex].present = false;
    users[emptyIndex].startTime = 0;
    users[emptyIndex].totalTime = 0;
  }
}

String formatTime(unsigned long seconds) {
  unsigned long hours = seconds / 3600;
  unsigned long minutes = (seconds % 3600) / 60;
  seconds = seconds % 60;
  return String(hours) + "h " + String(minutes) + "m " + String(seconds) + "s";
}

String generateHTML() {
  String html = "<!DOCTYPE html><html lang=\"en\">"
              "<head>"
              "<meta charset=\"UTF-8\">"
              "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
              "<title>Attendance System</title>"
              "<style>"
              "body {font-family: Arial, sans-serif;margin: 0;padding: 0;background-color: #f4f4f4;}"
              ".container {max-width: 800px;margin: 20px auto;padding: 20px;background-color: #fff;border-radius: 8px;box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);}"
              "h1 {color: #333;text-align: center;}"
              "ul {list-style-type: none;padding: 0;}"
              "li {margin-bottom: 10px;}"
              "button {padding: 10px 20px;background-color: #4CAF50;color: white;border: none;border-radius: 4px;cursor: pointer;}"
              "button:hover {background-color: #45a049;}"
              "</style>"
              "</head>"
              "<body>"
              "<div class=\"container\">"
              "<h1>Attendance List</h1>"
              "<ul>";

  for (int i = 0; i < numUsers; i++) {
    html += "<li>" + users[i].name + ": ";
    if (users[i].present) {
      html += "Present &#x2705; - Total Time: " + formatTime(users[i].totalTime + (now() - users[i].startTime));
    } else {
      html += "Absent &#x274C; - Total Time: " + formatTime(users[i].totalTime);
    }
    html += "</li>";
  }

  html += "</ul><form action=\"/reset\" method=\"POST\"><button type=\"submit\">Reset</button></form></div></body></html>";
  
  return html;
}

void resetData() {
  for (int i = 0; i < numUsers; i++) {
    users[i].present = false;
    users[i].startTime = 0;
    users[i].totalTime = 0;
  }
}

void setup() {
  Serial.begin(115200);
  SPI.begin();      // Start SPI bus
  mfrc522.PCD_Init();   // Start RFID reader

  // Set time
  setTime(0); // Set time to zero. In a real project, you should use NTP or RTC for real-time clock.

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected!");

  // Server settings
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", generateHTML());
  });

  server.on("/reset", HTTP_POST, [](AsyncWebServerRequest *request){
    resetData();
    request->send(200, "text/html", "<!DOCTYPE html>"
                                  "<html lang=\"en\">"
                                  "<head>"
                                  "<meta charset=\"UTF-8\">"
                                  "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
                                  "<title>Attendance System</title>"
                                  "</head>"
                                  "<body>"
                                  "<h1>Data Reset</h1>"
                                  "<a href=\"/\">Go back</a>"
                                  "</body>"
                                  "</html>");

  });

  server.begin();
}
void loop() {
  String rfid = readRFID();
  if (rfid != "") {
    Serial.println("Card read: " + rfid);
    for (int i = 0; i < numUsers; i++) {
      if (users[i].uid == rfid) {
        if (users[i].present) {
          // User exited, calculate attendance time
          users[i].totalTime += now() - users[i].startTime;
          users[i].present = false;
          Serial.println(users[i].name + " exited. Attendance time: " + formatTime(users[i].totalTime));
        } else {
          // User entered, record start time
          users[i].startTime = now();
          users[i].present = true;
          Serial.println(users[i].name + " entered.");
        }
      }
    }
  }
}
