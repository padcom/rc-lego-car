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
  startAP();
  connectWiFi();
}

void Network::run() {
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

void Network::startAP() {
  Serial.println("Starting access point " + config->getHardwareId());
  Serial.print("Access point IP: ");
  Serial.println(config->getAPLocalIP());

  WiFi.hostname(config->getHardwareId());
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(config->getAPLocalIP(), config->getAPGatewayIP(), config->getAPNetmask());
  WiFi.softAP(config->getHardwareId().c_str());

  Serial.println("Starting DNS server on port 53");
  dns->setErrorReplyCode(DNSReplyCode::NoError);
  dns->start(53, "*", config->getAPLocalIP());
}

bool Network::connectWiFi() {
  Serial.println("Connecting to WiFi");
  WiFi.config(config->getLocalIP(), config->getGatewayIP(), config->getNetmask(), config->getDNSServerIP());
  WiFi.begin(config->getWifiSsid().c_str(), config->getWifiPassword().c_str());
  if (WiFi.waitForConnectResult() == WL_CONNECTED) {
    Serial.println("WiFi connected. LocaIP: " + WiFi.localIP().toString());
    return true;
  } else {
    WiFi.disconnect();
    Serial.println("Unable to connect to WiFi - please update configuration");
    return false;
  }
}
