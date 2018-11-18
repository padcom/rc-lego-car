#include "Network.h"

bool Network::begin(String ssid, String password, IPAddress ip, IPAddress netmask, IPAddress gateway, IPAddress dns) {
  WiFi.config(ip, gateway, netmask, dns);
  WiFi.begin(ssid.c_str(), password.c_str());
  if (WiFi.waitForConnectResult() == WL_CONNECTED) {
    return true;
  } else {
    WiFi.disconnect();
    return false;
  }
}
