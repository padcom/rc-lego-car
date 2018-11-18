#ifndef _CONFIG_H
#define _CONFIG_H

#include <LinkedList.h>
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

    String app();
    String hardwareId();
    String string(String name, String def = "");
    uint16 port(String name, uint16 def = 0);
    IPAddress ip(String name);
    void set(String name, uint16 value);
    void set(String name, String value);
    void set(String name, IPAddress value);
  private:
    String appName;
    LinkedList<Option*> options;
    Option* get(String name);
};

#endif
