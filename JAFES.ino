#include "src/Leds.h"
#include "src/LocalServer.h"
#include "src/Laser.h"
#include "src/OTA.h"
#include "src/WorldClock.h"

#define SERIAL_BAUD 115200
#define USE_LEDS 1
#define ENABLE_OTA 0
#define SERVER_PORT 80
#define RETRY_DELAY 5
#define MAX_CONN_ATTEMPTS 20
#define SLEEP_START_HOUR 0
#define SLEEP_END_HOUR 9

#define ESP8266_WIFI_DEBUG 0

// 1 minutes:    60 000
//10 minutes:   600 000
//30 minutes: 1 800 000
// 1    hour: 3 600 000
#define NO_PLAY_DURATION 60000
#define PLAY_DURATION 600000
#define PLAY_LED_DURATION 3000

#define servoPin1 15 //D8
#define servoPin2 12 //D6
#define laserPin 5 //D1
#define laserPwm 99

const char* WF_SSID = "HoppaHoppaLeccici24"; //T-F07924
const char* WF_PWD = "Alzebra3";

Laser* laser;
LocalServer* localServer;

bool isPlaying = false;
bool sleepingMode = false;

bool isPlayLEDBlinking = false;
unsigned long playLEDLastChanged = 0;

unsigned long lastPlayEnd = 0;
unsigned long lastPlayBegun = 0;
unsigned long lastMove = 0;
unsigned long dTime = 0;

void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial.println("\n------------------------------------------------------");
  Serial.println("*************************");
  Serial.println("**     JUST BOOTED     **");
  Serial.println("*************************");

  Serial.print("Core debugging level: ");Serial.println(XCHAL_DEBUGLEVEL);

  if (USE_LEDS) {
    initLeds();
    tooglePowerLed(USE_LEDS);
  }
  if (!Wifi::setup(WF_SSID, WF_PWD, MAX_CONN_ATTEMPTS)) {
    Serial.println("ERROR: Without WiFi unable to advance forward!");
    Serial.print("Rebooting in ");Serial.print(RETRY_DELAY);Serial.print(" seconds.");
    for (int i = RETRY_DELAY; i > 0; i--) {
      Serial.print(".");
      Serial.print(".");
      delay(200);
      Serial.print(".");
      delay(200);
      Serial.print(".");
      delay(200);
      Serial.print(".");
      delay(200);
      Serial.print(".");
      if (i - 1 > 0) {
        delay(200);
        Serial.print(i - 1);
      }
    }
    
    Serial.println("Restarting ESP");
    delay(1000);
    ESP.restart();
    return;
  }
  toogleWifiLed(USE_LEDS);

  WorldClock::setup();
  
  laser = new Laser(laserPin, laserPwm);
  laser->initServos(servoPin1, servoPin2);
  localServer = new LocalServer(SERVER_PORT);

  if (ENABLE_OTA) {
    OTA::setup();
  }
}

void loop()
{
  unsigned long current = millis();

  if (ENABLE_OTA) {
    OTA::listen();
  }

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
    
  if (isItTimeToPlay(current) && !(sleepingMode && WorldClock::isItSleepTime(SLEEP_START_HOUR, SLEEP_END_HOUR))) {
    play(current);
  }
}

bool handleRequest(String request) {
  if (request.indexOf("GET /status/on") >= 0 && !isPlaying) {
      lastPlayEnd = millis() + NO_PLAY_DURATION;
      return true;
  } else if (request.indexOf("GET /status/off") >= 0 && isPlaying) {
      lastPlayBegun = millis() + PLAY_DURATION;
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

void play(unsigned long current) { 
  if (!isPlaying) {
    tooglePowerLed(USE_LEDS * isPlayLEDBlinking);
    playLEDLastChanged = current;
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

    if (current - playLEDLastChanged >= PLAY_LED_DURATION) {
      isPlayLEDBlinking = !isPlayLEDBlinking;
      tooglePowerLed(USE_LEDS * isPlayLEDBlinking);
      playLEDLastChanged = current;
    }

  } else {
    laser->off();
    isPlaying = false;
    lastPlayEnd = current;
    isPlayLEDBlinking = false;
    playLEDLastChanged = current;
    tooglePowerLed(USE_LEDS * isPlayLEDBlinking);
  }
}

bool isTimeOver(unsigned long current) {
  return current - lastPlayBegun >= PLAY_DURATION;
}

bool isItTimeToPlay(unsigned long current) {
  return current - lastPlayEnd >= NO_PLAY_DURATION;
}

bool isItAbleToMove(unsigned long current) {
  return current - lastMove >= dTime;
}
