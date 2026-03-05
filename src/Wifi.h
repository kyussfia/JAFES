#ifndef WIFI
#define WIFI
#include "Arduino.h"
#include "wl_definitions.h"
#include "HardwareSerial.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

namespace Wifi {

  char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

  bool scan(const char* ssid) {
    Serial.print("Scanning WiFi...");
    int n = WiFi.scanNetworks();
    Serial.printf("Found %d networks:\n", n);
    Serial.println("--------------------------------------------------");


    bool ssidFound = false;
    for (int i = 0; i < n; i++) {
      bool addExtraSpace = n > 9 && i < 10; 
      Serial.printf("%d:%s %s (%d dBm) %s\n", i + 1, (addExtraSpace ? " " : ""), WiFi.SSID(i).c_str(), WiFi.RSSI(i), (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? "OPEN" : "SECURED");
      if (WiFi.SSID(i) == ssid) {
        ssidFound = true;
      }
    }
    Serial.println("--------------------------------------------------");

    return ssidFound;
  }

  bool setup(const char* ssid, const char* pwd, int maxConnectionAttempts) {
    bool ssidFound = scan(ssid);
    if (!ssidFound) {
      Serial.print("ERROR: SSID (");Serial.print(ssid);Serial.println(") not found in scan!");
      return false;
    }
    Serial.println("Initiating WiFi connection:");

    WiFi.mode(WIFI_STA);
    WiFi.onStationModeDisconnected([](const WiFiEventStationModeDisconnected& event) {
      Serial.print("WiFi disconnect Reason code: ");Serial.println(event.reason);
    });
    WiFi.begin(ssid, pwd);

    Serial.print("Connecting to ");Serial.print(ssid);

    wl_status_t status = WiFi.status();
    for(int i = 0; i < maxConnectionAttempts && status != WL_CONNECTED; i++) {
      toogleWifiLed(true);
      delay(500);

      wl_status_t newStatus = WiFi.status();
      if (status != newStatus) {
        status = newStatus;
        Serial.print("Status: ");Serial.println(status);
      }
      Serial.print('.');
      toogleWifiLed(false);
      delay(500);
    }

    if (status != WL_CONNECTED) {
      Serial.println("Connection Failed!\n");
      return false;
    }

    Serial.print("\nConnected! IP address:\t\t");Serial.println(WiFi.localIP());

    return true;
  }
}

#endif