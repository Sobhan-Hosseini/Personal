/*
  counter_blink
  Author: Seyed Sobhan Hosseini
  Date: ‎Monday, ‎July ‎10, ‎2023, ‏‎7:18:58 PM
*/

const int ledPin = 13;
const unsigned long interval = 500;
int counter = 0;
unsigned long previousTime = 0; 
bool ledState = false;
bool pri = false; 

void setup(){
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT); 
}

void loop(){
  unsigned long currentTime = millis();

  if(currentTime - previousTime >= interval){
    previousTime = currentTime;
    pri = !pri;
    if (pri){
      Serial.println(counter); 
    }else{
      counter++; 
    }
  }
  
//  if (currentTime - previousTime >= interval) {
//    previousTime = currentTime; 
//    ledState = !ledState;
//    if (ledState) {
//      digitalWrite(ledPin, HIGH); 
//    } else {
//      digitalWrite(ledPin, LOW); 
//    }
//  }

}
