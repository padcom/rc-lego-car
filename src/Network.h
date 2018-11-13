#ifndef NETWORK_H
#define NETWORK_H

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include "Config.h"

class Network {
  public:
    Network(Config &config);
    ~Network();
    void begin();
    void run();
    void enableDebug();
    bool connectWiFi();
  private:
    bool debug = false;
    bool isConnecting = false;
    Config *config;
    DNSServer *dns;
    bool shouldEnterConfigMode();
    void startAP();
};

#endif
