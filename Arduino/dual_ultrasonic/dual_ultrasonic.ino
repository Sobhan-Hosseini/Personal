/*
  dual_ultrasonic
  Author: Seyed Sobhan Hosseini
  Date: ‎Wednesday, ‎July ‎19, ‎2023, ‏‎10:26:54 AM
*/

int trig1 = 5;
int trig2 = 8;
int echo = 6;
long time1 ;
int a = 1, b = 2, c = 3;
double dis;

void setup(){
pinMode(5, OUTPUT);
pinMode(6, INPUT);
Serial.begin(115200);
}

void loop(){
  digitalWrite(trig2, LOW);
  delayMicroseconds(2);
  digitalWrite(trig2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig2,LOW);
  digitalWrite(a,LOW);
  digitalWrite(b,LOW);
  digitalWrite(c,HIGH);
  time1 = pulseIn(echo, HIGH);
  dis = time1 * 0.034 / 2;
  Serial.print("distance 1: ");
  Serial.println(dis);
  delay(500);
  digitalWrite(trig1, LOW);
  delayMicroseconds(2);
  digitalWrite(trig1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig1,LOW);
  digitalWrite(a,LOW);
  digitalWrite(b,LOW);
  digitalWrite(c,LOW);
  time1 = pulseIn(echo, HIGH);
  dis = time1 * 0.034 / 2;
  Serial.print("distance 2: ");
  Serial.println(dis);
  delay(500);
}
