#include <Arduino.h>
#include <IPAddress.h>
#include <EEPROM.h>
#include "Config.h"
#include "SysLog.h"

#define IP_MAX_LENGTH           15
#define HOSTNAME_MAX_LENGTH     40
#define PORT_MAX_LENGTH          5
#define SECONDS_MAX_LENGTH       3

// Data structure holding configuration options in EEPROM
struct {
  char localIP[IP_MAX_LENGTH + 1];
  char netmask[IP_MAX_LENGTH + 1];
  char gatewayIP[IP_MAX_LENGTH + 1];
  char syslogHost[HOSTNAME_MAX_LENGTH + 1];
  char syslogPort[PORT_MAX_LENGTH + 1];
} data;

void readDataFromEEPROM() {
  EEPROM.begin(sizeof(data));
  EEPROM.get(0, data);
}

void saveDataToEEPROM() {
  EEPROM.put(0, data);
  EEPROM.commit();
}

Config::Config(String appName) {
  this->appName = appName;
}

void Config::begin() {
  readDataFromEEPROM();
  if (data.localIP[0] == 255) purge();
}

void Config::save() {
  SysLog.log("CONFIG Saving edited values");
  saveDataToEEPROM();
}

void Config::purge() {
  SysLog.log("CONFIG Resetting to factory defaults");

  memset(&data, 0, sizeof data);
  strncpy(data.syslogHost, "192.168.32.2", HOSTNAME_MAX_LENGTH);
  strncpy(data.syslogPort, "1514", HOSTNAME_MAX_LENGTH);
}

void Config::reset() {
  SysLog.log("CONFIG Resetting to default values");
}

String Config::getAppName() {
  return appName;
}

String Config::getHardwareId() {
  return appName + "-" + String(ESP.getChipId());
}

String Config::getWifiSsid() {
  return "padcom";
}

String Config::getWifiPassword() {
  return "siusiak13";
}

IPAddress strtoip(String ip) {
  IPAddress result;
  result.fromString(ip);
  return result;
}

IPAddress strtoip(char *ip) {
  IPAddress result;
  result.fromString(ip);
  return result;
}

IPAddress Config::getAPLocalIP() {
  return strtoip(String("192.168.4.1"));
}

IPAddress Config::getAPNetmask() {
  return strtoip(String("255.255.255.0"));
}

IPAddress Config::getAPGatewayIP() {
  return strtoip(String("192.168.4.1"));
}

IPAddress Config::getLocalIP() {
  return strtoip(data.localIP);
}

IPAddress Config::getNetmask() {
  return strtoip(data.netmask);
}

IPAddress Config::getGatewayIP() {
  return strtoip(data.gatewayIP);
}

IPAddress Config::getDNSServerIP() {
  return strtoip(String("8.8.8.8"));
}

IPAddress Config::getSyslogHost() {
  return strtoip(data.syslogHost);
}

uint16 Config::getSyslogPort() {
  return atoi(data.syslogPort);
}
