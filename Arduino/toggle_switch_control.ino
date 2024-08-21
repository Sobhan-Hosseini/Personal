bool re = false;
bool flag = true;
void setup() {
  pinMode(12,INPUT_PULLUP);
  pinMode(8,OUTPUT);
}

void loop() {
  if(flag){
    int counter = 0;
    while(digitalRead(12) == 0){
      counter = counter + 1;
      if(!digitalRead(12)){
        delay(20);
        if(!digitalRead(12)){
          digitalWrite(8,HIGH); 
        }
      }
      if(counter == 1) flag = !flag;
    }
  }
  else{
    int counter = 0;
     while(digitalRead(12) == 0){
      counter = counter + 1;
      if(!digitalRead(12)){
        delay(20);
        if(!digitalRead(12)){
          digitalWrite(8,LOW); 
        }
      }
      if(counter == 1) flag = !flag;
    }
  }
}
