#ifndef LEDS
#define LEDS
  #define POWER_LED LED_BUILTIN
  #define WIFI_LED 16

  void toogleLed(int led, bool on) {
    digitalWrite(led, on ? LOW : HIGH);
  }
  
  void initLeds() {
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(WIFI_LED, OUTPUT);
  }

  void tooglePowerLed(bool on) {
    toogleLed(LED_BUILTIN, on);
  }

  void toogleWifiLed(bool on) {
    toogleLed(WIFI_LED, on);
  }
#endif
