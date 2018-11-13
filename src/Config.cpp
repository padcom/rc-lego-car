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
  char ssid[32];
  char password[32];
  char localIP[IP_MAX_LENGTH + 1];
  char netmask[IP_MAX_LENGTH + 1];
  char gatewayIP[IP_MAX_LENGTH + 1];
  char dnsServerIP[IP_MAX_LENGTH + 1];
  char mqttHost[HOSTNAME_MAX_LENGTH + 1];
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
  return String(data.ssid);
}

void Config::setWifiSsid(String ssid) {
  memset(data.ssid, 0, sizeof(data.ssid));
  memcpy(data.ssid, ssid.c_str(), ssid.length());
}

String Config::getWifiPassword() {
  return String(data.password);
}

void Config::setWifiPassword(String password) {
  memset(data.password, 0, sizeof(data.password));
  memcpy(data.password, password.c_str(), password.length());
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

void Config::setLocalIP(String ip) {
  memset(data.localIP, 0, sizeof(data.localIP));
  memcpy(data.localIP, ip.c_str(), ip.length());
}

IPAddress Config::getNetmask() {
  return strtoip(data.netmask);
}

void Config::setNetmask(String netmask) {
  memset(data.netmask, 0, sizeof(data.netmask));
  memcpy(data.netmask, netmask.c_str(), netmask.length());
}

IPAddress Config::getGatewayIP() {
  return strtoip(data.gatewayIP);
}

void Config::setGatewayIP(String ip) {
  memset(data.gatewayIP, 0, sizeof(data.gatewayIP));
  memcpy(data.gatewayIP, ip.c_str(), ip.length());
}

IPAddress Config::getDNSServerIP() {
  return strtoip(String("8.8.8.8"));
}

void Config::setDNSServerIP(String ip) {
  memset(data.dnsServerIP, 0, sizeof(data.dnsServerIP));
  memcpy(data.dnsServerIP, ip.c_str(), ip.length());
}

IPAddress Config::getMqttHost() {
  return strtoip(data.mqttHost);
}

void Config::setMqttHost(String host) {
  memset(data.mqttHost, 0, sizeof(data.mqttHost));
  memcpy(data.mqttHost, host.c_str(), host.length());
}

IPAddress Config::getSyslogHost() {
  return strtoip(data.syslogHost);
}

void Config::setSyslogHost(String host) {
  memset(data.syslogHost, 0, sizeof(data.syslogHost));
  memcpy(data.syslogHost, host.c_str(), host.length());
}

uint16 Config::getSyslogPort() {
  return atoi(data.syslogPort);
}

void Config::setSyslogPort(uint16 port) {
  memset(data.syslogPort, 0, sizeof(data.syslogPort));
  itoa(port, data.syslogPort, 0);
}
