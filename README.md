# JAFES - Just Another Fun ESP8266 Server

A lightweight ESP8266-based web-controlled laser pointer system with sleep scheduling, OTA updates, and LED feedback.

Designed to serve a DIY entertainment system for my cats: Jack & Fillory.
The physical device was built on a ESP82 and 2 stepper engines to move a laser pointer with bond wires connected to the module. The laser pointer movement is random within its range.

## Features
- **Web Interface**: Control laser status and sleep mode via HTTP requests
- **Auto-Scheduling**: Automatically disables during sleep hours (00:00-09:00)
- **OTA Updates**: Wireless firmware updates via ArduinoOTA
- **LED Indicators**: Power and WiFi status LEDs
- **Random Movement**: Laser moves to random positions during active periods

## Hardware Requirements
- ESP8266 (NodeMCU/WeMos D1)
- Laser module (connected to GPIO 5/D1)
- Two servos (connected to GPIO 15/D8 and 12/D6)
- Power LED (built-in)
- WiFi LED (GPIO 16)

## Configuration
Edit these constants in `JAFES.ino`:
```cpp
#define WF_SSID "yourWifiSsid"
#define WF_PWD "yourWifiPwd"
#define SLEEP_START_HOUR 0
#define SLEEP_END_HOUR 9
#define PLAY_DURATION 600000  // 10 minutes active
#define NO_PLAY_DURATION 60000 // 1 minute cooldown
```

## Web Interface
Access via: `http://[ESP_IP]/`

Endpoints:
- `/status/on` - Activate laser
- `/status/off` - Deactivate laser
- `/sleep/on` - Enable sleep mode
- `/sleep/off` - Disable sleep mode

## Installation
1. Upload code to ESP8266
2. Connect laser and servos to designated pins
3. Power on and wait for WiFi connection
4. Access web interface at the device's IP address

## Notes
- Laser runs for 10 minutes, then cools down for 1 minute
- During sleep hours (00:00-09:00), laser remains off
- OTA updates available on port 8266