#ifndef NETWORK_H
#define NETWORK_H

#include <ESP8266WiFi.h>

class Network {
  public:
    void configure(String hostname, String ssid, String password, IPAddress ip, IPAddress netmask, IPAddress gateway, IPAddress dns);
    void startAP();
    bool connectWiFi();
  private:
    bool debug = false;
    bool isConnecting = false;
    String hostname;
    String ssid;
    String password;
    IPAddress ip;
    IPAddress netmask;
    IPAddress gateway;
    IPAddress dns;
};

#endif
