uint8_t AIR_Precharge = 2; 
uint8_t AIR_Main = 3;
uint8_t AIR_Negative = 4;
uint8_t AIR_Discharge = 5;
uint8_t LED_Discharge = 9;

uint8_t BMS = 7;
uint8_t Switch2 = 8;

bool carRunning = false;
bool prechargeFailed = false;
bool dischargeFinished = false;

unsigned long prechargeStart=-1;
const unsigned long prechargeTimeout = 5e6; // 5s ---amount of time that has to pass to mean the precharge failed
const unsigned long prechargeTime = 5e5; // 0.5s -- amount of time necessary to precharge
unsigned long lastValidStart = -1; 

unsigned long dischargeStart = -1;
const unsigned long dischargeInterval = 7.79e7; // 77.9s



void precharge() {
  /**
  Precharge function for the circuit.

  if precharge failed -> deactivates AIRS Precharge and Negative.
  if Optocoupler sends signal -> if has been precharging for enough time -> start car; deactivate AIR Precharge.
  */
  if (prechargeFailed){ 
    digitalWrite(AIR_Precharge, LOW);
    digitalWrite(AIR_Negative, LOW);
  }
  
    unsigned long now = micros();
    
    //This part waits for a steady signal from the optocoupler, 
    //current duration .5s but that's just a guess.
    if (lastValidStart == -1) lastValidStart = now;
    if (now - prechargeStart > prechargeTime) {
      carRunning = true;
      digitalWrite(AIR_Precharge, LOW);
    }
  } else{ 
      lastValidStart = -1;
      
      //timeout check
      if (now - prechargeStart > prechargeTimeout) { 
        prechargeFailed = true;
        fault();
      }
    }
}

void discharge() {
  digitalWrite(AIR_Discharge, HIGH);
  unsigned long now = micros();
  if (now - dischargeStart > dischargeInterval) {
    digitalWrite(LED_Discharge, HIGH);
    dischargeFinished = true;
  }
}

void fault() {
  //still not sure what this should do
}

void setup() {
  // put your setup code here, to run once:
  pinMode(AIR_Precharge, OUTPUT);    
  pinMode(AIR_Main, OUTPUT);
  pinMode(AIR_Negative, OUTPUT);
  pinMode(AIR_Discharge, OUTPUT);
  pinMode(LED_Discharge, OUTPUT);
  pinMode(Optocoupler, INPUT);
  
  pinMode(BMS, INPUT);
  pinMode(Switch2, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!carRunning) {
      if (digitalRead(Switch2) == HIGH) {
        if (prechargeStart == -1) {            
            digitalWrite(AIR_Precharge, HIGH);
            digitalWrite(AIR_Negative, HIGH);
            prechargeStart = micros();
            }
        precharge();
      }
  }
  else {
    if (!Switch2) {
      if (dischargeStart == -1) dischargeStart = micros();
      if (dischargeFinished == false) discharge();
    }
  }
}
