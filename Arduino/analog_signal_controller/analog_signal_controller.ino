int output;
void setup() {
  Serial.begin(9600);
  pinMode(A0 , INPUT);
  pinMode(6, OUTPUT);
}

void loop() {
  output = analogRead(A0);
  Serial.println(output);
  map(output, 0, 1023, 0, 255);
  analogWrite(6, output);
}
