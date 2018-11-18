#include <FS.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include "Config.h"
#include "ConfigManager.h"
#include "Network.h"
#include "SysLog.h"
#include "Servo.h"
#include "Button.h"

Config config("rclc");
Network network;
ESP8266WebServer server(80);

void configure() {
  config.addOption("ssid", "", 32);
  config.addOption("password", "", 32);
  config.addOption("ip", "", 15);
  config.addOption("netmask", "", 15);
  config.addOption("gateway", "", 15);
  config.addOption("dns", "8.8.8.8", 15);
  config.addOption("syslog-host", "", 15);
  config.addOption("syslog-port", "514", 5);
  config.addOption("mqtt-host", "", 15);
  config.addOption("mqtt-port", "1883", 5);
  config.load();
}

bool shouldStartConfigurationManager() {
  Button cfg(D0);
  return cfg.pressed();
}

String getConfig() {
  return config.serialize();
}

bool setConfig(String data) {
  config.parse(data);
  if (network.begin(config.string("ssid"), config.string("password"), config.ip("ip"), config.ip("netmask"), config.ip("gateway"), config.ip("dns"))) {
    config.save();
    return true;
  } else {
    Serial.println("Settings NOT saved.");
    config.load();
    return false;
  }
}

void connect() {
  if (shouldStartConfigurationManager()) {
    Serial.println("Starting configuration manager at http://192.168.4.1/cp/");
    ConfigManager.start(config.hardwareId(), IPAddress(192,168,4,1), getConfig, setConfig);
  } else {
    if (!network.begin(config.string("ssid"), config.string("password"), config.ip("ip"), config.ip("netmask"), config.ip("gateway"), config.ip("dns"))) {
      Serial.println("Unable to connect to WiFi - please update configuration at http://192.168.4.1/cp/");
      ConfigManager.start(config.hardwareId(), IPAddress(192,168,4,1), getConfig, setConfig);
    } else {
      Serial.println("WiFi connected. LocaIP: " + WiFi.localIP().toString());
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("\nRemote Controlled Lego Car (RCLC) firmware 1.0");

  EEPROM.begin(512);
  SPIFFS.begin();

  // Setup the configuration
  configure();

  // Manage network connection
  connect();

  // Configure logging
  SysLog.begin(config.ip("syslog-host"), config.port("syslog-port"), config.app(), config.hardwareId());

  // Default handlers
  server.serveStatic("/", SPIFFS, "/");
  server.onNotFound([]() {
    server.send(200, "text/plain", String(server.method()) + " " + server.uri() + " not found\n");
  });
  // Start the web server
  server.begin();
}

void loop() {
  server.handleClient();
}
