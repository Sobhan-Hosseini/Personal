double highTime = 0;
double lowTime = 0;
unsigned long cycleTime = 0;
int pin = D1;
unsigned long firstTime;
unsigned long start;
unsigned long downtime;
unsigned long upTime;
unsigned long totalTime;
unsigned long t = 0;

double dutyCycle;
int counter = 2;

bool state = false;
void IRAM_ATTR change();

void setup(){
  Serial.begin(115200);
  pinMode(pin , INPUT);
  attachInterrupt(digitalPinToInterrupt(pin), change, CHANGE);
}

void loop(){
  firstTime = micros();
  while(state == true){
    upTime = micros() - firstTime;
  } 
  start = micros();
  while(state == false){
    downtime = micros() - start;
  }
   
  dutyCycle = upTime / (upTime + downtime);
  Serial.println(dutyCycle);
  Serial.println("an");
  }
  
  void IRAM_ATTR change(){
  state = !state;
}
