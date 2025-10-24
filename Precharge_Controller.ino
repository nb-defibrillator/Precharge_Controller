uint8_t AIR_Precharge = 2;
uint8_t AIR_Main = 3;
uint8_t AIR_3 = 4;

uint8_t Optocoupler = 5;
uint8_t BMS = 6;


bool CarRunning;

void setup() {
  // put your setup code here, to run once:
  pinMode(AIR_Precharge, OUTPUT);    
  pinMode(AIR_Main, OUTPUT);
  pinMode(AIR_3, OUTPUT);
  pinMode(Optocoupler, INPUT);
  pinMode(BMS, INPUT);
  CarRunning = false;
}

void loop() {
  // put your main code here, to run repeatedly:

}
