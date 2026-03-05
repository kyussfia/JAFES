#ifndef WORLDCLOCK
#define WORLDCLOCK
#include <NTPClient.h>
#include <WiFiUdp.h>

#define UTC_OFFSET_SEC 3600

namespace WorldClock {

  WiFiUDP ntpUDP;
  NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", UTC_OFFSET_SEC);

  void setup() {
    timeClient.begin();
  }

  bool isItSleepTime(int from, int to) {
    timeClient.update();
    int hours = timeClient.getHours();
    return hours >= from && hours <= to;
  }
}

#endif
