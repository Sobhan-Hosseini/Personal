#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN D6
#define LED_COUNT 8
int r = 0;
int g = 0;
int b = 0;
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();

}

void loop() {
  strip.clear();
  r = 128;
  g = 0;
  b = 128;
  
  while(r > -1 && b < 256){
    for(int i = 0; i < 8; i++){
      strip.setPixelColor(i, r, g, b);
      strip.show();
      }
      r = r - 1;
      b = b + 1;
      delay(10);
    }
    r = 0;
    g = 0;
    b = 255; 
    while(g < 256 && b > -1){
      for(int i = 0; i < 8; i++){
        strip.setPixelColor(i, r, g, b);
        strip.show();
      }
      g = g + 1;
      b = b - 1;
      delay(10);
    }
    r = 0;
    g = 255;
    b = 0;
    while(r<256){
      for(int i = 0; i < 8; i++){
        strip.setPixelColor(i, r, g, b);
        strip.show();
      }
      r = r + 1;
      delay(10);
    }
    r = 255;
    g = 255;
    b = 0;
    while(g > -1){
      for(int i = 0; i < 8; i++){
        strip.setPixelColor(i, r, g, b);
        strip.show();
      }
      g = g - 1;
      delay(10);
    }
    r = 255;
    g = 0;
    b = 0;
    while(r > 127 && b < 129){
      for(int i = 0; i < 8; i++){
        strip.setPixelColor(i, r, g, b);
        strip.show();
      }
      r = r - 1;
      b = b + 1;
      delay(10);
    }
    r = 128;
    g = 0;
    b = 128;
  delay(10);
}
