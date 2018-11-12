#include "Arduino.h"
#include "SysLog.h"

#define MAX_LOG_MESSAGE  256

SysLogClient SysLog;

void SysLogClient::begin(IPAddress host, uint16_t port, String app, String source) {
  this->host = host;
  this->port = port;
  this->app = app;
  this->source = source;
  this->initialized = true;
}

void SysLogClient::debug(const String &message) {
  #ifdef DEBUG
  log(LOG_INFO, message);
  #endif
}

void SysLogClient::debugf(const String &message, ...) {
  #ifdef DEBUG
  char msg[MAX_LOG_MESSAGE];
  va_list arg;
  va_start(arg, message);
  vsnprintf(msg, sizeof(msg), message.c_str(), arg);
  va_end(arg);
  log(LOG_INFO, String(msg));
  #endif
}

void SysLogClient::log(const String &message) {
  log(LOG_INFO, message);
}

void SysLogClient::logf(const String &message, ...) {
  char msg[MAX_LOG_MESSAGE];
  va_list arg;
  va_start(arg, message);
  vsnprintf(msg, sizeof(msg), message.c_str(), arg);
  va_end(arg);
  log(LOG_INFO, String(msg));
}

void SysLogClient::log(const uint16 level, const String &message) {
  log(LOG_DAEMON, level, message);
}

void SysLogClient::logf(const uint16 level, const String &message, ...) {
  char msg[MAX_LOG_MESSAGE];
  va_list arg;
  va_start(arg, message);
  vsnprintf(msg, sizeof(msg), message.c_str(), arg);
  va_end(arg);
  log(LOG_DAEMON, level, String(msg));
}

void SysLogClient::log(const uint16 facility, const uint16 level, const String &message) {
  if (!initialized) {
    Serial.println(F("SYSLOG NOT INITIALIZED! Call begin() first."));
    return;
  }

  udp.beginPacket(host, port);
  udp.printf("<%d>1 - - - %s %s - %s\n", facility + level, source.c_str(), app.c_str(), message.c_str());
  udp.endPacket();
  udp.flush();

  Serial.println(message);
}

void SysLogClient::logf(const uint16 facility, const uint16 level, const String &message, ...) {
  char msg[MAX_LOG_MESSAGE];
  va_list arg;
  va_start(arg, message);
  vsnprintf(msg, sizeof(msg), message.c_str(), arg);
  va_end(arg);
  log(facility, level, String(msg));
}
