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
Countimer fanTimer;

double minTargetTemp = 28.00;
double tempAdjustmentRange = 4.0;

int mistPeriod = 15;
int mistLoop = 0;
bool mistOn = false;

int fanPeriod = 5;
int fanLoop = 0;
bool fanOn = false;

void setup() {
  Serial.begin(9600);
  pinMode(fan, OUTPUT);
  pinMode(mist, OUTPUT);
  pinMode(heater, OUTPUT);
  mistTimer.setInterval(makeMist, 1000);
  fanTimer.setInterval(runFan, 1000);
}

void makeMist() {
  mistLoop++;
  if(!mistOn) {
    if(mistLoop > (60 - mistPeriod)) {
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

void runFan() {
  fanLoop++;
  if(!fanOn) {
    if(fanLoop > (60 - fanPeriod)) {
      fanOn = !fanOn;
      fanLoop = 0;
    }
  } else {
    if(fanLoop > fanPeriod) {
      fanOn = !fanOn;
      fanLoop = 0;
    }
  }
}

void loop() {
  mistTimer.run();
  if(!mistTimer.isCounterCompleted()) {
    mistTimer.start();
  }

  fanTimer.run();
  if(!fanTimer.isCounterCompleted()) {
    fanTimer.start();
  }

  if(mistOn) {
    digitalWrite(mist, HIGH);
  } else {
    digitalWrite(mist, LOW);
  }

  if(fanOn) {
    digitalWrite(fan, HIGH);
  } else {
    digitalWrite(fan, LOW);
  }
}
