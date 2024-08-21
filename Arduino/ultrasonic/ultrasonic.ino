double duration = 0;
double distance = 0;
void setup() {
  Serial.begin(115200);
  pinMode(3 , OUTPUT);
  pinMode(2 , INPUT);
}

void loop() {
  digitalWrite(3, LOW);
  delayMicroseconds(2);
  digitalWrite(3, HIGH);
   delayMicroseconds(10);
  digitalWrite(3, LOW);
  duration = pulseIn(2, HIGH);
  distance = duration * 0.034 / 2;
  Serial.println(distance);
  delay(1);
}
