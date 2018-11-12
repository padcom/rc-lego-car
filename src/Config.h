#ifndef _CONFIG_H
#define _CONFIG_H

#include <WiFiManager.h>
#include <IPAddress.h>

class Config {
  public:
    Config(String appName);
    void begin();
    void configure(WiFiManager &manager);
    void save();
    void purge();
    void reset();
    String getAppName();
    String getHardwareId();
    IPAddress getLocalIP();
    IPAddress getNetmask();
    IPAddress getGatewayIP();
    IPAddress getSyslogHost();
    uint16 getSyslogPort();
  private:
    String appName;
    WiFiManagerParameter *localIP;
    WiFiManagerParameter *netmask;
    WiFiManagerParameter *gatewayIP;
    WiFiManagerParameter *syslogHost;
    WiFiManagerParameter *syslogPort;
};

#endif
