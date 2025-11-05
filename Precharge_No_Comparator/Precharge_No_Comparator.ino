/**
Authors:
Natu Benyam Demeke
Violet Enslow
Ryanne Wilson


Precharge Controller
No comparator; instead, when switch 2 is flipped, closes the Precharge AIR for 7 seconds before opening it and then closing the Main AIR.
*/

#define ULONG_MAX 4294967295UL //Serves as a 'null' state for the timers

//pin IDs
const uint8_t AIR_Precharge = 2; 
const uint8_t AIR_Main = 3;
const uint8_t AIR_Discharge = 5;
const uint8_t LED_Discharge = 9;
const uint8_t LED_Fault = 10;

const uint8_t Optocoupler = 14;
uint8_t BMS = 7;
uint8_t Switch2 = 8;

bool carRunning = false; //True when the car can start driving ; as in, when precharging has finished and is successful.
bool prechargeFailed = false; //True if the precharge failed.
bool dischargeFinished = false; // True if the discharge finished!

//All measurements of time are in milliseconds!

unsigned long prechargeStart = ULONG_MAX; // The time at which precharge started; in millis; ULONG_MAX acts as a 'null' here.
const unsigned long prechargeTimeoutInterval = 10e4; // 10s ---amount of time that has to pass to mean the precharge failed; in millis
const unsigned long prechargeInterval = 1.5e4; // 1.5s -- amount of time necessary to precharge; in millis

unsigned long optocouplerActivatedStart = ULONG_MAX; // The time at which Optocoupler was active; in millis

unsigned long dischargeStart = ULONG_MAX;
const unsigned long dischargeInterval = 7.79e7; // 77.9s; the amount of time it takes to discharge; in millis;

void setup() {
  // put your setup code here, to run once:
  pinMode(AIR_Precharge, OUTPUT);     // Normally open- LOW = open, HIGH = closed
  pinMode(AIR_Main, OUTPUT);          // Normally open
  pinMode(AIR_Discharge, OUTPUT);     // Normally open
  pinMode(LED_Discharge, OUTPUT);
  pinMode(Optocoupler, INPUT_PULLUP); //Uses internal pullup resistors; default HIGH -> active LOW
  pinMode(LED_Fault, OUTPUT);
  
  pinMode(BMS, INPUT);
  pinMode(Switch2, INPUT);
}

void precharge() {

    unsigned long now = micros();
    
    if (now - prechargeStart > prechargeTime) { //If it has been 7 seconds
      //Sets the car running; opens the precharge AIR, closes the main AIR
      carRunning = true;
      digitalWrite(AIR_Precharge, LOW);
      digitalWrite(AIR_Main, HIGH);
    }

}

void loop() {
  // put your main code here, to run repeatedly:
  if (carRunning == false) {
      if (digitalRead(Switch2) == HIGH ) { //&& digitalRead(BMS) == HIGH
        if (prechargeStart == ULONG_MAX) {   //if the prechargeStart hasn't yet been assigned         
            digitalWrite(AIR_Precharge, HIGH); //Closes AIR precharge
            prechargeStart = millis();
            }
        precharge();
      }
  }
}

