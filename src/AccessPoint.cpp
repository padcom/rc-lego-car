#include "AccessPoint.h"

void AccessPointManager::start(String hostname, IPAddress ip) {
  IPAddress mask = IPAddress(255,255,255,0);

  WiFi.hostname(hostname);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(ip, ip, mask);
  WiFi.softAP(hostname.c_str());
}

void AccessPointManager::stop() {
  WiFi.softAPdisconnect();
}

AccessPointManager AccessPoint;
