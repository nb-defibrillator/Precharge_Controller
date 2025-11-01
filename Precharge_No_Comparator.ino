uint8_t AIR_Precharge = 2; 
uint8_t AIR_Main = 3;
uint8_t AIR_Negative = 4;
uint8_t AIR_Discharge = 5;
uint8_t LED_Discharge = 9;

uint8_t BMS = 7;
uint8_t Switch2 = 8;

bool carRunning = false;

unsigned long prechargeStart = 0;
const unsigned long prechargeTime = 5e6; // 5s -- amount of time necessary to precharge 

void setup() {
  // put your setup code here, to run once:
  pinMode(AIR_Precharge, OUTPUT);    
  pinMode(AIR_Main, OUTPUT);
  pinMode(AIR_Negative, OUTPUT);
  pinMode(AIR_Discharge, OUTPUT);
  pinMode(LED_Discharge, OUTPUT);
  
  pinMode(BMS, INPUT);
  pinMode(Switch2, INPUT);
}

void precharge() {

    unsigned long now = micros();
    
    if (now - prechargeStart > prechargeTime) {
      carRunning = true;
      digitalWrite(AIR_Precharge, LOW);
    }

}

void fault() {
  //still not sure what this should do
}



void loop() {
  // put your main code here, to run repeatedly:
  if (!carRunning) { //Car is not running
      if (digitalRead(Switch2) == HIGH) {
        if (prechargeStart == -1) {            
            digitalWrite(AIR_Precharge, HIGH);
            digitalWrite(AIR_Negative, HIGH);
            prechargeStart = micros();
            }
        precharge();
      }
  }
}
