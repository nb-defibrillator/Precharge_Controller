#define ULONG_MAX 4294967295UL

uint8_t AIR_Precharge = 2; 
uint8_t AIR_Main = 3;
uint8_t AIR_Negative = 4;
uint8_t AIR_Discharge = 5;
uint8_t LED_Discharge = 9;

uint8_t Optocoupler = 6;
uint8_t BMS = 7;
uint8_t Switch2 = 8;

bool carRunning = false;
bool prechargeFailed = false;
bool dischargeFinished = false;

//All measurements of time are in milliseconds!


unsigned long prechargeStart = ULONG_MAX; // The time at which precharge started; in millis
const unsigned long prechargeTimeoutInterval = 10e4; // 10s ---amount of time that has to pass to mean the precharge failed; in millis
const unsigned long prechargeInterval = 1.5e4; // 1.5s -- amount of time necessary to precharge; in millis

unsigned long optocouplerActivatedStart = ULONG_MAX; // The time at which Optocoupler was active; in millis

unsigned long dischargeStart = ULONG_MAX;
const unsigned long dischargeInterval = 7.79e7; // 77.9s; the amount of time it takes to discharge; in millis;

void setup() {
  // put your setup code here, to run once:
  pinMode(AIR_Precharge, OUTPUT);     // Normally open- LOW = open, HIGH = closed
  pinMode(AIR_Main, OUTPUT);          // Normally open
  pinMode(AIR_Negative, OUTPUT);      // Normally open
  pinMode(AIR_Discharge, OUTPUT);     // Normally open
  //TODO: Add Fault LED
  pinMode(LED_Discharge, OUTPUT);
  pinMode(Optocoupler, INPUT_PULLUP); //Uses internal pullup resistors; default HIGH -> active LOW
  
  pinMode(BMS, INPUT);
  pinMode(Switch2, INPUT);
}

void precharge() {
  /**
  Precharge function for the circuit.

  if precharge failed -> deactivates AIRS Precharge and Negative.
  if Optocoupler sends signal -> if has been precharging for enough time -> start car; deactivate AIR Precharge.
  */

  if (digitalRead(Optocoupler) == LOW) { //if Optocoupler is active
    unsigned long now = millis();
    
    //This part waits for a steady signal from the optocoupler, 
    //current duration .5s but that's just a guess.
    if (optocouplerActivatedStart == ULONG_MAX) optocouplerActivatedStart = now;

    unsigned long chargingInterval = now - optocouplerActivatedStart;

    if (chargingInterval > prechargeInterval) {
      carRunning = true;
      digitalWrite(AIR_Precharge, LOW);
    }
  } 

  //timeout check
  unsigned long timeoutInterval = now - prechargeStart;
  if (timeoutInterval > prechargeTimeoutInterval) { 
    // Timed out; precharge has failed! fault :(
    prechargeFailed = true;
    digitalWrite(AIR_Precharge, LOW);
    digitalWrite(AIR_Negative, LOW);
    digitalWrite(AIR_Main, LOW);
    digitalWrite(AIR_Discharge, LOW);
    fault();
  }
}

void discharge() {
  digitalWrite(AIR_Discharge, HIGH);
  unsigned long now = millis();
  if (now - dischargeStart > dischargeInterval) {
    digitalWrite(LED_Discharge, HIGH);
    dischargeFinished = true;
  }
}

void fault() {
  //still not sure what this should do
}

void loop() {
  // put your main code here, to run repeatedly:
  if (carRunning == false) {
      if (digitalRead(Switch2) == HIGH ) { //&& digitalRead(BMS) == HIGH
        if (prechargeStart == ULONG_MAX) {            
            digitalWrite(AIR_Precharge, HIGH);
            digitalWrite(AIR_Negative, HIGH);
            prechargeStart = millis();
            }
        precharge();
      }
  }
  // else {
  //   if (!Switch2) {
  //     if (dischargeStart == ULONG_MAX) dischargeStart = millis();
  //     if (dischargeFinished == false) discharge();
  //   }
  // }
}
