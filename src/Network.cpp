#include <WiFiManager.h>
#include "Network.h"
#include "SysLog.h"

#define CONFIG_PIN  13

Network::Network(Config &config) {
  this->config = &config;
}

void Network::begin() {
  connectWiFi();
}

void Network::run() {
  if (!WiFi.isConnected()) {
    connectWiFi();
  }
}

void Network::enableDebug() {
  debug = true;
}

bool Network::shouldEnterConfigMode() {
  // Pin D7 on Wemos D1 mini
  pinMode(CONFIG_PIN, INPUT_PULLUP);
  return digitalRead(CONFIG_PIN) == 0;
}

void Network::connectWiFi() {
  WiFiManager manager;
  manager.setDebugOutput(debug);
  config->configure(manager);
  if (shouldEnterConfigMode()) {
    config->reset();
    manager.setDebugOutput(true);
    manager.resetSettings();
    if (manager.startConfigPortal(config->getHardwareId().c_str())) {
      config->save();
    }
  } else {
    manager.setSTAStaticIPConfig(config->getLocalIP(), config->getGatewayIP(), config->getNetmask());
    if (!manager.begin()) {
      ESP.reset();
    } else {
      SysLog.log("WiFi connected");
    }
  }
}
