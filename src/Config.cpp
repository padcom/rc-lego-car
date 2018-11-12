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

  this->localIP = new WiFiManagerParameter("local-ip", "Local IP", data.localIP, IP_MAX_LENGTH);
  this->netmask = new WiFiManagerParameter("netmask", "Netmask", data.netmask, IP_MAX_LENGTH);
  this->gatewayIP = new WiFiManagerParameter("gateway-ip", "Gateway IP", data.gatewayIP, IP_MAX_LENGTH);
  this->syslogHost = new WiFiManagerParameter("syslogh", "Syslog host", data.syslogHost, HOSTNAME_MAX_LENGTH);
  this->syslogPort = new WiFiManagerParameter("syslogp", "Syslog port", data.syslogPort, PORT_MAX_LENGTH);
}

void Config::configure(WiFiManager &manager) {
  manager.addParameter(localIP);
  manager.addParameter(netmask);
  manager.addParameter(gatewayIP);
  manager.addParameter(syslogHost);
  manager.addParameter(syslogPort);
}

void Config::save() {
  SysLog.log("CONFIG Saving edited values");

  strncpy(data.localIP, localIP->getValue(), localIP->getValueLength());
  strncpy(data.netmask, netmask->getValue(), netmask->getValueLength());
  strncpy(data.gatewayIP, gatewayIP->getValue(), gatewayIP->getValueLength());
  strncpy(data.syslogHost, syslogHost->getValue(), syslogHost->getValueLength());
  strncpy(data.syslogPort, syslogPort->getValue(), syslogPort->getValueLength());

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

IPAddress strtoip(char *ip) {
  IPAddress result;
  result.fromString(ip);
  return result;
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

IPAddress Config::getSyslogHost() {
  return strtoip(data.syslogHost);
}

uint16 Config::getSyslogPort() {
  return atoi(data.syslogPort);
}
