/*
  Traffic Light
  Author: Seyed Sobhan Hosseini
  Date: Wednesday, August 9, 2023, 4:05:40 PM
*/

#include <ESP8266WiFi.h>
 
const char* ssid = "Robotics";
const char* password = "@Auriga1399#";
 
int green = D1;
int yellow = D2;
int red = D3; 

WiFiServer server(80);
 
void setup(){
  Serial.begin(115200);
  pinMode(green, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(red, OUTPUT);
  digitalWrite(green, LOW);
  digitalWrite(yellow, LOW);
  digitalWrite(red, LOW);
 
  Serial.print("Connecting to Internet ");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
 
  server.begin();
  Serial.println("Server started");
 
  Serial.print("IP Address of network: ");
  Serial.println(WiFi.localIP());
  Serial.print("Copy and paste the following URL: https://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop(){
    WiFiClient client = server.available();    
    if (!client){
      return;
    }
  Serial.println("Waiting for new client");   
  while(!client.available()){
    delay(1);
  }
 
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  bool gvalue = HIGH;
  bool yvalue = HIGH;
  bool rvalue = HIGH;

  if (request.indexOf("/green=ON") != -1){
    digitalWrite(green, HIGH);
    gvalue = HIGH;
  }
  if (request.indexOf("/green=OFF") != -1){
    digitalWrite(green, LOW);
    gvalue = LOW;
  }
  if (request.indexOf("/yellow=ON") != -1){
    digitalWrite(yellow, HIGH);
    yvalue = HIGH;
  }
  if (request.indexOf("/yellow=OFF") != -1){
    digitalWrite(yellow, LOW);
    yvalue = LOW;
  } 
    if (request.indexOf("/red=ON") != -1){
    digitalWrite(red, HIGH);
    rvalue = HIGH;
  }
  if (request.indexOf("/red=OFF") != -1){
    digitalWrite(red, LOW);
    rvalue = LOW;
  } 

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); 
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  client.print("<title>**The traffic light**</title>");

  client.println("<div>GREEN : </div>");
  client.println("<a href=\"/green=ON\"\"><button>ON</button></a>");
  client.println("<a href=\"/green=OFF\"\"><button>OFF</button></a><br />");  
  client.println("<br>");
  client.println("<div>YELLOW : </div>");
  client.println("<a href=\"/yellow=ON\"\"><button>ON</button></a>");
  client.println("<a href=\"/yellow=OFF\"\"><button>OFF</button></a><br />");  
  client.println("<br>");
  client.println("<div>RED : </div>");
  client.println("<a href=\"/red=ON\"\"><button>ON</button></a>");
  client.println("<a href=\"/red=OFF\"\"><button>OFF</button></a><br />");  
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}
