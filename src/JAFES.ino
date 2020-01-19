#include "Leds.h"
#include "LocalServer.h"
#include "Laser.h"

#define servoPin1 15 //D8
#define servoPin2 12 //D6
#define laserPin 5 //D1
#define laserPwm 70

bool useStatusLeds = false;
Laser* laser;
LocalServer* localServer;

bool isPlaying = false;
bool sleepingMode = false;
unsigned long lastPlayEnd = 0;
unsigned long lastPlayBegun = 0;
unsigned long lastMove = 0;
const unsigned long nonPlayTime = 3600000; //30 minutes 1800000 // 1 hour 3 600 000
const unsigned long playTime = 600000; //10 minutes 600000
unsigned long dTime = 0;

void setup() {
  if (useStatusLeds) {
    initLeds();
    tooglePowerLed(true);
  }
  connectToWifi(useStatusLeds);
  initLaser();
  localServer = new LocalServer(80);
}

void loop()
{
  unsigned long current = millis();
  OTA();
  WiFiClient client = localServer->listen();  
  if (client) {
    String request = localServer->receive(client);
    bool shouldRedirect = handleRequest(request);
    Serial.print("Should redirect ");Serial.println(shouldRedirect);
    if (request.indexOf("TIMEOUT") >= 0) {
      localServer->timeout(client);
      localServer->close(client);
    } else if (shouldRedirect) {
      localServer->redirect(client);
      localServer->close(client);
    } else {
      localServer->respond(client, isPlaying, sleepingMode);
      localServer->close(client); 
    }
  }
    
  if (isItTimeToPlay(current) && (!sleepingMode || !sleepTime())) {
    play(current);
  }
}

//sleep time  if hour is between 00 and 09
bool sleepTime() {
  timeClient.update();
  int hours = timeClient.getHours();
  return hours >= 0 && hours <= 9;
}

bool handleRequest(String request) {
  if (request.indexOf("GET /status/on") >= 0 && !isPlaying) {
      lastPlayEnd = millis() + nonPlayTime;
      return true;
  } else if (request.indexOf("GET /status/off") >= 0 && isPlaying) {
      lastPlayBegun = millis() + playTime;
      return true;
  } else if (request.indexOf("GET /sleep/on") >= 0 && !sleepingMode) {
      sleepingMode = true;
      return true;
  } else if (request.indexOf("GET /sleep/off") >= 0 && sleepingMode) {
      sleepingMode = false;
      return true;
  }
  return false;
}

void initLaser() {
  laser = new Laser(laserPin, laserPwm);
  laser->initServos(servoPin1, servoPin2);
}

void play(unsigned long current) { 
  if (!isPlaying) {
    lastPlayBegun = current;
  }
  isPlaying = true;
  if (!isTimeOver(current)) {
    laser->on();
    if (isItAbleToMove(current)) {
      laser->moveSeparate(random(0, 31), 90 + random(0, 91));
      lastMove = current;
      dTime = random(0, 4000);
    }     
  } else {
    laser->off();
    isPlaying = false;
    lastPlayEnd = current;
  }
}

bool isTimeOver(unsigned long current) {
  return current - lastPlayBegun >= playTime;
}

bool isItTimeToPlay(unsigned long current) {
  return current - lastPlayEnd >= nonPlayTime;
}

bool isItAbleToMove(unsigned long current) {
  return current - lastMove >= dTime;
}
