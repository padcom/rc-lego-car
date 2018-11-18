#include "AccessPoint.h"

AccessPoint::AccessPoint(String hostname, IPAddress ip) {
  this->hostname = hostname;
  this->ip = ip;
}

void AccessPoint::start() {
  IPAddress mask = IPAddress(255,255,255,0);

  WiFi.hostname(hostname);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(ip, ip, mask);
  WiFi.softAP(hostname.c_str());
}

void AccessPoint::stop() {
  WiFi.softAPdisconnect();
}
