#ifndef _CONFIG_H
#define _CONFIG_H

#include <IPAddress.h>

struct Option;

class Config {
  public:
    Config(String appName);
    void addOption(String name, String value, int length);
    void load();
    void save();
    void reset();
    void parse(String data);
    String serialize();

    String getAppName();
    String getHardwareId();
    String getString(String name, String def = "");
    void setString(String name, String value);
    uint16 getPort(String name, uint16 def = 0);
    void setPort(String name, uint16 value);
    IPAddress getIpAddress(String name);
    void setIpAddress(String name, IPAddress value);
  private:
    String appName;
    Option* options = 0;
    Option* get(String name);
    size_t size();
};

#endif
