#ifndef NETWORK_H
#define NETWORK_H

#include <ESP8266WiFi.h>

class Network {
  public:
    bool begin(String ssid, String password, IPAddress ip, IPAddress netmask, IPAddress gateway, IPAddress dns);
};

#endif
