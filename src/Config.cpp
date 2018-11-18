#include <Arduino.h>
#include <EEPROM.h>
#include <IPAddress.h>
#include "Config.h"
#include "SysLog.h"

struct Option {
  String name;
  String value;
  int length;
};

Config::Config(String appName) {
  this->appName = appName;
}

void Config::addOption(String name, String value, int length) {
  Option* option = new Option;
  option->name = name;
  option->value = value;
  option->length = length;

  options.add(option);
}

void Config::load() {
  SysLog.log("CONFIG Loading saved values");
  int address = 0;
  for (int i = 0; i < options.size(); i++) {
    Option* option = options.get(i);
    char buffer[option->length];
    for (int j = 0; j < option->length; j++) {
      buffer[j] = EEPROM.read(address + j);
    }
    option->value = String(buffer);
    address += option->length;
  }
}

void Config::save() {
  SysLog.log("CONFIG Saving new configuration values");
  int address = 0;
  for (int i = 0; i < options.size(); i++) {
    Option* option = options.get(i);
    for (int j = 0; j < option->length; j++) {
      EEPROM.write(address + j, option->value.charAt(j));
    }
    address += option->length;
  }
  EEPROM.commit();
}

void Config::reset() {
  SysLog.log("CONFIG Resetting to default values");
  for (int i = 0; i < options.size(); i++) {
    options.get(i)->value = "";
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
      set(name, value);
    }
  }
}

String Config::serialize() {
  String result = "";
  for (int i = 0; i < options.size(); i++) {
    Option* option = options.get(i);
    String entry = option->name + "=" + option->value + "\n";
    result += entry;
  }
  return result;
}

String Config::app() {
  return appName;
}

String Config::hardwareId() {
  return appName + "-" + String(ESP.getChipId());
}

String Config::string(String name, String def) {
  Option* option = get(name);
  return option == 0 ? def : option->value;
}

uint16 Config::port(String name, uint16 def) {
  Option* option = get(name);
  return option == 0 ? def : atoi(option->value.c_str());
}

IPAddress Config::ip(String name) {
  Option* option = get(name);
  IPAddress ip;
  if (option) ip.fromString(option->value);
  return ip;
}

void Config::set(String name, String value) {
  Option* option = get(name);
  if (option) option->value = value;
}

void Config::set(String name, uint16 value) {
  Option* option = get(name);
  if (option) option->value = String(value);
}

void Config::set(String name, IPAddress value) {
  Option* option = get(name);
  if (option) option->value = value.toString();
}

Option* Config::get(String name) {
  for (int i = 0; i < options.size(); i++) {
    Option* option = options.get(i);
    if (option->name.compareTo(name) == 0) return option;
  }
  return NULL;
}
