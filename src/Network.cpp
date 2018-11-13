#include "Network.h"

void Network::configure(String hostname, String ssid, String password, IPAddress ip, IPAddress netmask, IPAddress gateway, IPAddress dns) {
  this->hostname = hostname;
  this->ssid = ssid;
  this->password = password;
  this->ip = ip;
  this->netmask = netmask;
  this->gateway = gateway;
  this->dns = dns;
}

void Network::startAP() {
  IPAddress ip = IPAddress(192,168,4,1);
  IPAddress mask = IPAddress(255,255,255,0);
  Serial.println("Starting access point " + hostname);
  Serial.println("Access point IP: " + ip.toString());

  WiFi.hostname(hostname);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(ip, ip, mask);
  WiFi.softAP(hostname.c_str());
}

bool Network::connectWiFi() {
  Serial.println("Connecting to WiFi");
  WiFi.config(ip, gateway, netmask, dns);
  WiFi.begin(ssid.c_str(), password.c_str());
  if (WiFi.waitForConnectResult() == WL_CONNECTED) {
    Serial.println("WiFi connected. LocaIP: " + WiFi.localIP().toString());
    return true;
  } else {
    WiFi.disconnect();
    Serial.println("Unable to connect to WiFi - please update configuration at http://192.168.4.1/cp/");
    return false;
  }
}
