#include "Network.h"
#include "SysLog.h"

#define CONFIG_PIN  D2 //13

Network::Network(Config &config) {
  this->config = &config;
  this->dns = new DNSServer();
}

Network::~Network() {
  delete this->dns;
}

void Network::begin() {
  connectWiFi();
}

void Network::run() {
  if (!WiFi.isConnected()) {
    connectWiFi();
  }
  dns->processNextRequest();
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
  WiFi.hostname(config->getHardwareId());
  WiFi.mode(WIFI_AP_STA);

  WiFi.softAPConfig(config->getAPLocalIP(), config->getAPGatewayIP(), config->getAPNetmask());
  WiFi.softAP(config->getHardwareId().c_str());
  dns->setErrorReplyCode(DNSReplyCode::NoError);
  dns->start(53, "*", config->getAPLocalIP());

  WiFi.config(config->getLocalIP(), config->getGatewayIP(), config->getNetmask(), config->getDNSServerIP());
  WiFi.begin(config->getWifiSsid().c_str(), config->getWifiPassword().c_str());
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Unable to connect to WiFi");
  } else {
    Serial.println("Connected.");
  }
}
