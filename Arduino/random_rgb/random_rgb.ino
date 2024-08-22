/*
  random_rgb
  Author: Seyed Sobhan Hosseini
  Date: ‎Wednesday, ‎July ‎12, ‎2023, ‏‎12:58:20 PM
*/

int red = 2;
int green = 4;
int blue = 7;

void setup(){
  Serial.begin(9600);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A5, OUTPUT);
}

void loop(){
   delay(500);
   int rb = random(0,255);
   int rg = random(0,255);
   int rr = random(0,255);
   Serial.println(rb);
   if(rb > 127){
    digitalWrite(blue, HIGH);
   }else{
    digitalWrite(blue, LOW);
   }

   if(rg > 127){
    digitalWrite(green, HIGH);
   }else{
    digitalWrite(green, LOW);
   }

   if(rr > 127){
    digitalWrite(red, HIGH);
   }else{
    digitalWrite(red, LOW);
   }

   analogWrite(A1, rr);
   analogWrite(A3, rg);
   analogWrite(A5, rb);
}
