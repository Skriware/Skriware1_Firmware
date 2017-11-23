
#define   F_CPU   1000000UL

#define RPI_POWER_PIN 2
#define POWER_SUPPLY_PIN 8
#define POWER_BTN_PIN 5

#define DELAY 100
#define POWERCUT_COUNTDOWN 100*DELAY // 10 sec
#define FORCESHUT_COUNTDOWN 60*DELAY // 6 sec

boolean rpiPowered = false;
boolean rpiUp = false;
boolean btnBlocked = false; 

int powerOffCountdown = POWERCUT_COUNTDOWN;
int forcePowerOffCountdown = FORCESHUT_COUNTDOWN;

void setup() {
  // put your setup code here, to run once:
  pinMode(POWER_BTN_PIN, INPUT_PULLUP);

  pinMode(POWER_SUPPLY_PIN, OUTPUT);
  digitalWrite(POWER_SUPPLY_PIN, LOW);

  pinMode(RPI_POWER_PIN, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

   /****************************************
   * POWER MANAGMENT
   ****************************************/
  // HIGH when RPi successfully booted into program
  // and set pullup resistor
  if(digitalRead(RPI_POWER_PIN))
    rpiUp = true;
   
  // step into when btn is pressed
  if (!digitalRead(POWER_BTN_PIN)) {
    // signal RPi that btn is pressed
    pinMode(RPI_POWER_PIN, OUTPUT);
    digitalWrite(RPI_POWER_PIN, LOW);
    
    // if RPi is off, turn it on
    if (!rpiPowered && !btnBlocked) {
      digitalWrite(POWER_SUPPLY_PIN, HIGH);
      rpiPowered = true;
    }
    else { 
      // if rpi is on and btn is being pressed 
      // start count down for force shutdown
      forcePowerOffCountdown -= DELAY;
    }
  } 
  else {
    // if btn not pressed switch to listen on this pin
    // and reset force shutdown timer
    pinMode(RPI_POWER_PIN, INPUT);
    forcePowerOffCountdown = FORCESHUT_COUNTDOWN;
    btnBlocked = false;
  }

  // check if raspberry is shutting down, 
  // if so start countdown to power cut
  if (!digitalRead(RPI_POWER_PIN) && rpiUp)
    powerOffCountdown -= DELAY;
  else
    powerOffCountdown = POWERCUT_COUNTDOWN;

  // cut the power if either of countdowns goes to 0
  // and reset flags and countdowns
  if (powerOffCountdown <= 0 || forcePowerOffCountdown <= 0) {
    digitalWrite(POWER_SUPPLY_PIN, LOW);
    powerOffCountdown = POWERCUT_COUNTDOWN;
    forcePowerOffCountdown = FORCESHUT_COUNTDOWN;
    rpiPowered = false;
    rpiUp = false;
    btnBlocked = true;
  }

  unsigned long previousMillis = millis();
  unsigned long currentMillis = millis();
  while(currentMillis - previousMillis < DELAY) {
    currentMillis = millis();
  }
} //loop
