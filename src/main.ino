#include "Config.h"
#include "Network.h"
#include "SysLog.h"
#include "StatusIndicator.h"
#include "Servo.h"
#include "Button.h"
#include <FS.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

Config config("rclc");
Network network(config);
Button s1left(D1);
Button s1right(D2);
Button s2left(D3);
Button s2right(D4);
Servo s1;
Servo s2;
ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  Serial.println("\nRemote Controlled Lego Car (RCLC) firmware 1.0");
  config.begin();
  Status.begin();
  // network.enableDebug();
  network.begin();
  SysLog.begin(
    config.getSyslogHost(), config.getSyslogPort(),
    config.getAppName(), config.getHardwareId());
  s1.attach(D0);
  s2.attach(D5);

  SPIFFS.begin();
  FSInfo info;
  if (SPIFFS.info(info)) {
    Serial.print("SPIFFS: totalBytes = "); Serial.println(info.totalBytes);
    Serial.print("SPIFFS: usedBytes  = "); Serial.println(info.usedBytes);
  } else {
    Serial.println("SPIFFS: error - cannot read filesystem information");
  }

  // handle captive portal to enable configuration over wifi
  server.on("/hotspot-detect.html", HTTP_GET, []() {
    server.sendHeader("Location", String("/cp/"), true);
    server.send(302, "text/plain", "Redirecting to captive portal");
  });

  // API
  server.on("/api/reset", HTTP_GET, []() {
    server.send(200, "text/plain", "Restarting...");
    server.handleClient();
    delay(1000);
    ESP.reset();
  });
  server.on("/api/config", HTTP_GET, []() {
    StaticJsonBuffer<500> jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["ssid"] = config.getWifiSsid();
    json["passwd"] = config.getWifiPassword();
    json["ip"] = config.getLocalIP().toString();
    json["netmask"] = config.getNetmask().toString();
    json["gateway"] = config.getGatewayIP().toString();
    json["dns"] = config.getDNSServerIP().toString();
    json["syslogh"] = config.getSyslogHost().toString();
    json["syslogp"] = config.getSyslogPort();
    json["mqtt"] = "192.168.32.2";
    server.sendHeader("Access-Control-Allow-Origin", "*");
    String response;
    json.prettyPrintTo(response);
    server.send(200, "application/json", response);
  });
  server.on("/api/config", HTTP_OPTIONS, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Credentials", "false");
    server.sendHeader("Access-Control-Allow-Headers", "X-Requested-With,Content-Type");
    server.sendHeader("Access-Control-Allow-Methods", "GET,POST,OPTIONS");
    server.send(204);
  });
  server.on("/api/config", HTTP_POST, []() {
    StaticJsonBuffer<500> jsonBuffer;
    JsonObject& json = jsonBuffer.parse(server.arg("plain"));
    String data;
    json.prettyPrintTo(Serial);
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/plain", "OK");
  });
  server.on("/api/heap", HTTP_GET, []() {
    server.send(200, "text/plain", String(ESP.getFreeHeap()));
  });
  server.on("/api/test", HTTP_GET, []() {
    server.send(200, "text/plain", "test = " + server.arg("test") + "\n");
  });

  // Default handlers
  server.serveStatic("/", SPIFFS, "/");
  server.onNotFound([]() {
    server.send(200, "text/plain", String(server.method()) + " " + server.uri() + " not found\n");
  });

  // Start the web server
  server.begin();
}

// long ts = millis();

void loop() {
  if (s1left.pressed()) {
    s1.writeMicroseconds(1000);
  } else if (s1right.pressed()) {
    s1.writeMicroseconds(2000);
  } else {
    s1.writeMicroseconds(1500);
  }
  if (s2left.pressed()) {
    s2.writeMicroseconds(1000);
  } else if (s2right.pressed()) {
    s2.writeMicroseconds(2000);
  } else {
    s2.writeMicroseconds(1500);
  }

  if (s1left.pressed() || s1right.pressed() || s2left.pressed() || s2right.pressed()) {
    Status.on();
  } else {
    Status.off();
  }

  // if (millis() - ts > 1000) {
  //   ts = millis();
  //   Serial.print("HEAP=");
  //   Serial.println(ESP.getFreeHeap());
  // }

  network.run();
  server.handleClient();
}
