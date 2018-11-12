#ifndef NETWORK_H
#define NETWORK_H

#include <ESP8266WiFi.h>
#include "Config.h"

class Network {
  public:
    Network(Config &config);
    void begin();
    void run();
    void enableDebug();
  private:
    bool debug = false;
    Config *config;
    bool shouldEnterConfigMode();
    void connectWiFi();
};

#endif
