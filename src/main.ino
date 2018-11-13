#include "Config.h"
#include "Network.h"
#include "SysLog.h"
#include "StatusIndicator.h"
#include "Servo.h"
#include "Button.h"
#include "HttpServer.h"
#include "MimeType.h"

Config config("rclc");
Network network(config);
Button s1left(D1);
Button s1right(D2);
Button s2left(D3);
Button s2right(D4);
Servo s1;
Servo s2;
HttpServer http(80, "/index.html");

void handleApiGet(HttpRequest &request, HttpResponse &response) {
  response.status(200, "OK - root handler");
  response.header("Content-Type", "text/plain");
  response.beginBody();
  response.printf("test.html: %s\ntest.css: %s\ntest.js: %s\n",
    getMimeType("test.html").c_str(),
    getMimeType("test.css").c_str(),
    getMimeType("test.js").c_str());
  response.endBody();
}

void handleApiPost(HttpRequest &request, HttpResponse &response) {
  response.status(200, "OK");
  response.header("Content-Type", request.getHeader("Content-Type"));
  response.header("Content-Length", request.getHeader("Content-Length", "0"));
  response.beginBody();
  response.print(request.getBody());
  response.endBody();
}

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

  http.registerHandler("GET", "/api/test", handleApiGet);
  http.registerHandler("POST", "/api/test", handleApiPost);
  http.begin();
}

long ts = millis();

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

  if (millis() - ts > 1000) {
    ts = millis();
    Serial.print("HEAP=");
    Serial.println(ESP.getFreeHeap());
  }

  http.run();
  network.run();
}
