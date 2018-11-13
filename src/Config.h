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
    String getWifiPassword();
    IPAddress getAPLocalIP();
    IPAddress getAPNetmask();
    IPAddress getAPGatewayIP();
    IPAddress getLocalIP();
    IPAddress getNetmask();
    IPAddress getGatewayIP();
    IPAddress getDNSServerIP();
    IPAddress getSyslogHost();
    uint16 getSyslogPort();
  private:
    String appName;
};

#endif
