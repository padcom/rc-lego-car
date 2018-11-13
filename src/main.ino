#include "Config.h"
#include "Network.h"
#include "SysLog.h"
#include "Servo.h"
#include "Button.h"
#include <FS.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <EEPROM.h>

Config config("rclc");
Network network;
DNSServer dns;
ESP8266WebServer server(80);

void redirectToCaptivePortal() {
  Serial.println("GET /hotspot-detect.html");
  server.sendHeader("Location", String("/cp/"), true);
  server.send(302, "text/plain", "Redirecting to captive portal");
}

void retrieveConfiguration() {
  Serial.println("GET /api/config");
  server.send(200, "application/text", config.serialize());
}

void configureNetwork() {
  network.configure(
    config.getHardwareId(),
    config.getString("ssid"),
    config.getString("password"),
    config.getIpAddress("ip"),
    config.getIpAddress("netmask"),
    config.getIpAddress("gateway"),
    config.getIpAddress("dns"));
}

void uploadConfiguration() {
  Serial.println("POST /api/config");
  config.parse(server.arg("plain"));
  configureNetwork();
  if (network.connectWiFi()) {
    config.save();
    server.send(200, "text/plain", "Connected and settings saved");
  } else {
    config.load();
    configureNetwork();
    network.connectWiFi();
    server.send(500, "text/plain", "Cannot connect to wifi - settings not saved");
  }
}

void checkNetworkConnectivity() {
  server.send(200, "text/plain", WiFi.isConnected() ? "Connected" : "Not connected");
}

void notFoundHandler() {
  server.send(200, "text/plain", String(server.method()) + " " + server.uri() + " not found\n");
}

void setup() {
  EEPROM.begin(512);
  SPIFFS.begin();

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

  SysLog.begin(
    config.getIpAddress("syslog-host"), config.getPort("syslog-port"),
    config.getAppName(), config.getHardwareId());

  Serial.begin(115200);
  Serial.println("\nRemote Controlled Lego Car (RCLC) firmware 1.0");

  configureNetwork();
  network.startAP();
  network.connectWiFi();

  // DNS server is needed so that captive portal knows it is not connected to the internet
  dns.setErrorReplyCode(DNSReplyCode::NoError);
  dns.start(53, "*", WiFi.softAPIP());

  // handle captive portal to enable configuration over wifi
  server.on("/hotspot-detect.html", HTTP_GET, redirectToCaptivePortal);
  // API
  server.on("/api/config", HTTP_GET, retrieveConfiguration);
  server.on("/api/config", HTTP_POST, uploadConfiguration);
  server.on("/api/connected", HTTP_GET, checkNetworkConnectivity);
  // Default handlers
  server.serveStatic("/", SPIFFS, "/");
  server.onNotFound(notFoundHandler);
  // Start the web server
  server.begin();
}

void loop() {
  server.handleClient();
}
