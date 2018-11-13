#ifndef _CONFIG_H
#define _CONFIG_H

#include <IPAddress.h>

class Config {
  public:
    Config(String appName);
    void begin();
    void save();
    void purge();
    void reset();
    String getAppName();
    String getHardwareId();
    String getWifiSsid();
    void setWifiSsid(String ssid);
    String getWifiPassword();
    void setWifiPassword(String password);
    IPAddress getAPLocalIP();
    IPAddress getAPNetmask();
    IPAddress getAPGatewayIP();
    IPAddress getLocalIP();
    void setLocalIP(String ip);
    IPAddress getNetmask();
    void setNetmask(String netmask);
    IPAddress getGatewayIP();
    void setGatewayIP(String ip);
    IPAddress getDNSServerIP();
    void setDNSServerIP(String ip);
    IPAddress getMqttHost();
    void setMqttHost(String host);
    IPAddress getSyslogHost();
    void setSyslogHost(String host);
    uint16 getSyslogPort();
    void setSyslogPort(uint16 port);
  private:
    String appName;
};

#endif
