#ifndef ACCESSPOINT_H
#define ACCESSPOINT_H

#include <ESP8266WiFi.h>

class AccessPointManager {
  public:
    void start(String hostname, IPAddress ip);
    void stop();
};

extern AccessPointManager AccessPoint;

#endif
