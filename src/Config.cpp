#include <Arduino.h>
#include <EEPROM.h>
#include <IPAddress.h>
#include "Config.h"
#include "SysLog.h"

struct Option {
  String name;
  String value;
  int length;
  Option *next = 0;
};

Config::Config(String appName) {
  this->appName = appName;
}

void Config::addOption(String name, String value, int length) {
  Option* option = new Option;
  option->name = name;
  option->value = value;
  option->length = length;
  option->next = 0;

  Option* opts = options;
  if (opts) {
    while (opts->next) opts = opts->next;
    opts->next = option;
  } else {
    options = option;
  }
}

void Config::load() {
  Option* opts = options;
  int address = 0;
  while (opts) {
    char buffer[opts->length];
    for (int j = 0; j < opts->length; j++) {
      buffer[j] = EEPROM.read(address + j);
    }
    opts->value = String(buffer);
    address += opts->length;
    opts = opts->next;
  }
}

void Config::save() {
  SysLog.log("CONFIG Saving edited values");
  Option* opts = options;
  int address = 0;
  while (opts) {
    for (int j = 0; j < opts->length; j++) {
      EEPROM.write(address + j, opts->value.charAt(j));
    }
    address += opts->length;
    opts = opts->next;
  }
  EEPROM.commit();
}

void Config::reset() {
  SysLog.log("CONFIG Resetting to default values");
  Option* opts = options;
  while (opts) {
    opts->value = "";
    opts = opts->next;
  }
}

void Config::parse(String data) {
  while (data.length() > 2) {
    int index = data.indexOf("\n");
    index = index == -1 ? data.length() : index;
    String option = data.substring(0, index);
    data.remove(0, index + 1);
    index = option.indexOf("=");
    if (index >= 1) {
      String name = option.substring(0, index);
      String value = option.substring(index + 1, option.length());
      setString(name, value);
    }
  }
}

String Config::serialize() {
  String result = "";
  Option* opt = options;
  while (opt) {
    String entry = opt->name + "=" + opt->value + "\n";
    result += entry;
    opt = opt->next;
  }
  return result;
}

String Config::getAppName() {
  return appName;
}

String Config::getHardwareId() {
  return appName + "-" + String(ESP.getChipId());
}

String Config::getString(String name, String def) {
  Option* option = get(name);
  return option == 0 ? def : option->value;
}

void Config::setString(String name, String value) {
  Option* option = get(name);
  if (option) option->value = value;
}

uint16 Config::getPort(String name, uint16 def) {
  Option* option = get(name);
  return option == 0 ? def : atoi(option->value.c_str());
}

void Config::setPort(String name, uint16 value) {
  Option* option = get(name);
  if (option) option->value = String(value);
}

IPAddress Config::getIpAddress(String name) {
  Option* option = get(name);
  IPAddress ip;
  if (option) ip.fromString(option->value);
  return ip;
}

void Config::setIpAddress(String name, IPAddress value) {
  Option* option = get(name);
  if (option) option->value = value.toString();
}

Option* Config::get(String name) {
  Option* opts = options;
  while (opts) {
    if (opts->name.compareTo(name) == 0) return opts;
    opts = opts->next;
  }
  return 0;
}

size_t Config::size() {
  Option* opts = options;
  size_t size = 0;
  while (opts) {
    size += opts->length;
    opts = opts->next;
  }
  return size;
}
