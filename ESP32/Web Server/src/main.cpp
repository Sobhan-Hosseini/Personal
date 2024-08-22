/*
  Web Server
  Author: Seyed Sobhan Hosseini
  Date: Tuesday, July 25, 2023, 3:01:31 PM
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>


const char* ssid = "Robotics";
const char* password = "@Auriga1399#"; 

ESP8266WebServer server(80);
void handle_OnConnect();

String SendHTML(uint8_t hello){
  String ptr = "<!DOCTYPE html>\n";
  ptr +="<html>\n";
  ptr +="<head>\n";
  // ptr +="<title>LED Control</title>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<p>Helloooooooooooo :)</p>\n";
  // ptr +="<form method=\"get\">\n";
  // if(led)
  // ptr +="<input type=\"button\" value=\"LED OFF\" onclick=\"window.location.href='/ledoff'\">\n";
  // else
  // ptr +="<input type=\"button\" value=\"LED ON\" onclick=\"window.location.href='/ledon'\">\n";
  // ptr +="</form>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}

  void handle_OnConnect(){
    server.send(200, "text/html", SendHTML(false)); 
  }

void setup(){
  Serial.begin(115200);
  delay(100);

  Serial.println("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(){
  server.handleClient();
  handle_OnConnect();
}
