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

int sampleSize = 50;
float avgTemp[50];

double targetTemp = 30.00;
double currentTemp, pidOutput;
PID heatingPID(&currentTemp, &pidOutput, &targetTemp, 0.5, 7, 1, DIRECT);

int i = 0;

void setup() {
  heatingPID.SetMode(AUTOMATIC);
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

double getTemp() {
  double total = 0;
  double rawvoltage= analogRead(temp);
  double millivolts= (rawvoltage/1024.0) * 5000;
  double celsius= millivolts/10;
  for ( i=0; i<sampleSize-1; i++) {
    avgTemp[i] = avgTemp[i+1];
  }
  avgTemp[sampleSize-1] = celsius;
  for ( i=0; i<sampleSize; i++) {
    total = total + avgTemp[i];
  }
  double average = total / sampleSize;
  return average;
}

void loop() {
  mistTimer.run();
  if(!mistTimer.isCounterCompleted()) {
    mistTimer.start();
  }

  mistPeriod = analogRead(humidityDial) * 0.04;
  currentTemp = getTemp(); 
  heatingPID.Compute();
  analogWrite(heater, pidOutput);

  if(mistOn) {
    digitalWrite(mist, HIGH);
    digitalWrite(fan, HIGH);
  } else {
    digitalWrite(mist, LOW);
    digitalWrite(fan, LOW);
  }

}
