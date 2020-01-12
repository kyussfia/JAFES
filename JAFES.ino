#include "Leds.h"
#include "Wifi.h"
#include "Laser.h"

#define servoPin1 15 //D8
#define servoPin2 12 //D6
#define laserPin 5 //D1
#define laserPwm 70

bool useStatusLeds = true;
Laser* laser;

bool isPlaying = false;
unsigned long lastPlayEnd = 0;
unsigned long lastPlayBegun = 0;
const unsigned long nonPlayTime = 10000; //30 minutes 1800000
const unsigned long playTime = 20000; //10 minutes 600000

void setup() {
  if (useStatusLeds) {
    initLeds();
    tooglePowerLed(true);
  }
  connectToWifi(useStatusLeds);
  initLaser();
}

void loop()
{
  OTA();
  unsigned long currentTime = millis();
  if (isItTimeToPlay(currentTime)) {
    play(currentTime);
  }
}

void initLaser() {
  laser = new Laser(laserPin, laserPwm);
  laser->initServos(servoPin1, servoPin2);
}

void play(unsigned long currentTime) {
  if (!isPlaying) {
    lastPlayBegun = currentTime;
  }
  isPlaying = true;
  if (!isTimeOver(currentTime)) {
    laser->on();
    laser->move(90);
    laser->move(180);
    laser->move(0);
  } else {
    laser->off();
    isPlaying = false;
    lastPlayEnd = currentTime;
  }
}

bool isTimeOver(unsigned long currentTime) {
  return currentTime - lastPlayBegun > playTime;
}

bool isItTimeToPlay(unsigned long currentTime) {
  return currentTime - lastPlayEnd > nonPlayTime;
}
