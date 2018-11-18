#ifndef ACCESSPOINT_H
#define ACCESSPOINT_H

#include <ESP8266WiFi.h>

class AccessPoint {
  public:
    AccessPoint(String hostname, IPAddress ip);
    void start();
    void stop();
  private:
    String hostname;
    IPAddress ip;
};

#endif
