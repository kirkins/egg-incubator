#include <Countimer.h>
#include <PID_v1.h>

//INPUTS
#define tempDial A0
#define humidityDial A1
#define temp A2

//OUTPUTS
#define fan 5
#define mist 6
#define heater 9

Countimer mistTimer;

double minTargetTemp = 28.00;
double tempAdjustmentRange = 4.0;

double mistPeriod = 0.0;
int mistLoop = 0;
bool mistOn = false;

void setup() {
  Serial.begin(9600);
  pinMode(fan, OUTPUT);
  pinMode(mist, OUTPUT);
  pinMode(heater, OUTPUT);
  mistTimer.setInterval(makeMist, 1000);
}

void makeMist() {
  mistLoop++;
  if(!mistOn) {
    if(mistLoop > (60 - mistPeriod) && mistPeriod > 0) {
      mistOn = !mistOn;
      mistLoop = 0;
    }
  } else {
    if(mistLoop > mistPeriod) {
      mistOn = !mistOn;
      mistLoop = 0;
    }
  }
}

void loop() {
  mistTimer.run();
  if(!mistTimer.isCounterCompleted()) {
    mistTimer.start();
  }

  mistPeriod = analogRead(humidityDial) * 0.04;

  if(mistOn) {
    digitalWrite(mist, HIGH);
    digitalWrite(fan, HIGH);
  } else {
    digitalWrite(mist, LOW);
    digitalWrite(fan, LOW);
  }
}
