#include<Timer.h>

// timers for doing something
Timer timeForControl;
Timer timeForCommunicate;
Timer timeForReadSensor;

// to store the targets
double temTarget[16];
double rhTarget[17];
int co2Target[16];
int lightTarget[16];

int lightNowTarget;
double temNowTarget, deltaTem;
double rhNowTarget, deltaRh;
int co2NowTarget, deltaCo2;

// to adjust parameters
/*2013
double deltaTemUpperLimit = 0.5, deltaTemLowerLimit = -0.3;
double deltaRhLowerLimit = -5.0;
int deltaCo2Limit = 50;*/
/*2014
double deltaTemUpperLimit = 0.1, deltaTemLowerLimit = -0.4;
double deltaRhLowerLimit = -5.0;
int deltaCo2Limit = 50;*/
double deltaTemUpperLimit = 0.2, deltaTemLowerLimit = -0.4;
double deltaRhLowerLimit = -4.0;
int deltaCo2Limit = 50;

// parametmers
int light1Open = 0;
int light2Open = 0;
int light3Open = 0;
int CO2Open = 0;
int heaterOpen = 0;
int humidifierOpen = 0;
int heatpumpOpen = 0;

int heatpumpMark = 0;
int heatpumpMark2 = 0;
int heatpumpMark3 = 0;

int humidifierHigh = 0;
int heaterHigh = 0;
int heatpumpHigh = 0;

double humidifierTemporary = 0;
double heaterTemporary = 0;
double humidifierTemporary1 = 0;
double heaterTemporary1 = 0;

int humidifierTime = 0;
int heaterTime = 0;
int heatpumpTime = 0;
int pumpTime = 0;
int co2DelayTime = 3;
int coolPoint = 10;
int coolTime = 25;
int co2Time = co2DelayTime;

int humidifierStopTime = 0;
int heaterStopTime = 0;
int heatpumpStopTime = 0;


// to store datas from sensor
double co2, rh, tem;
double LastCo2, LastRh, LastTem;

// timer
bool controlOrNot = false;
int timer = 0, count = 0;

// pins for control plants
int light1 = 2;
int light2 = 3;
int light3 = 4;
int CO2 = 5;
int heatpump = 6;
int compressor = 7;
int humidifier = 8;
int fan = 9;
int heater = 10;

void setup() {
  timeForControl.every(1000, control);
  timeForCommunicate.every(150, communicateWithPC);
  timeForReadSensor.every(50, readSensor);
  
  Serial.begin(115200); // for every print
  Serial3.begin(28800); // for every sensor reading
  
  pinMode(light1, OUTPUT);
  pinMode(light2, OUTPUT);
  pinMode(light3, OUTPUT);
  pinMode(CO2, OUTPUT);
  pinMode(heatpump, OUTPUT);
  pinMode(compressor, OUTPUT);
  pinMode(humidifier, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(heater, OUTPUT);

  digitalWrite(light1 , LOW);
  digitalWrite(light2 , LOW);
  digitalWrite(light3 , LOW);
  digitalWrite(CO2 , LOW);
  digitalWrite(heatpump , LOW);
  digitalWrite(compressor , HIGH);
  digitalWrite(humidifier , LOW);
  digitalWrite(fan , HIGH);
  digitalWrite(heater, LOW);
}

void loop()
{
  // stop every control when reach 15 minutes
  if (count > 15 && (timer%60) == 5) {
    digitalWrite(light1 , LOW);
    digitalWrite(light2 , LOW);
    digitalWrite(light3 , LOW);
    digitalWrite(CO2 , LOW);
    digitalWrite(heatpump , LOW);
    digitalWrite(compressor , LOW);
    digitalWrite(humidifier , LOW);
    digitalWrite(fan , LOW);
    digitalWrite(heater, LOW);
  }
  else
    timeForControl.update();
  
  timeForCommunicate.update();
  timeForReadSensor.update();
}
