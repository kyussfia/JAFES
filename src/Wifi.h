#ifndef WIFI
#define WIFI

  #include <ESP8266WiFi.h>
  #include <NTPClient.h>
  #include <ESP8266mDNS.h>
  #include <WiFiUdp.h>
  #include <ArduinoOTA.h>
  #include "Leds.h"

  const char* ssid = "T-F07924";
  const char* pwd = "9rp39hW6MNScdv73";
  const long utcOffsetInSeconds = 3600;
  char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

  const short attemptsToConnect = 20;

  WiFiUDP ntpUDP;
  NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", utcOffsetInSeconds);

  void connectToWifi(bool useStatusLeds) {
    Serial.begin(115200);         // Start the Serial communication to send messages to the computer
    Serial.println("Booting");
    Serial.println('\n');
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pwd);
    Serial.print("Connecting to ");Serial.print(ssid);

    for(int i = 0; i < attemptsToConnect && WiFi.status() != WL_CONNECTED; i++) {
      toogleWifiLed(true);
      delay(500);
      Serial.print('.');
      toogleWifiLed(false);
      delay(500);
    }

    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Connection Failed! Rebooting...");
      delay(3000);
      ESP.restart();
    }

    timeClient.begin();

    if (useStatusLeds) {
      toogleWifiLed(true);
    }

    // Port defaults to 8266
    // ArduinoOTA.setPort(8266);
  
    // Hostname defaults to esp8266-[ChipID]
    // ArduinoOTA.setHostname("myesp8266");
  
    // No authentication by default
    // ArduinoOTA.setPassword("admin");
  
    // Password can be set with it's md5 value as well
    // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
    // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

   ArduinoOTA.onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH) {
        type = "sketch";
      } else { // U_FS
        type = "filesystem";
      }
  
      // NOTE: if updating FS this would be the place to unmount FS using FS.end()
      Serial.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
      Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) {
        Serial.println("Auth Failed");
      } else if (error == OTA_BEGIN_ERROR) {
        Serial.println("Begin Failed");
      } else if (error == OTA_CONNECT_ERROR) {
        Serial.println("Connect Failed");
      } else if (error == OTA_RECEIVE_ERROR) {
        Serial.println("Receive Failed");
      } else if (error == OTA_END_ERROR) {
        Serial.println("End Failed");
      }
    });
    ArduinoOTA.begin();
    Serial.println("\nConnected! IP address:\t");  
    Serial.println(WiFi.localIP());
  }

  void OTA() {
    ArduinoOTA.handle();
  }
#endif
