#include "Config.h"
#include "Network.h"
#include "SysLog.h"
#include "StatusIndicator.h"
#include "Servo.h"
#include "Button.h"
#include "HttpServer.h"

Config config("rclc");
Network network(config);
Button s1left(D1);
Button s1right(D2);
Button s2left(D3);
Button s2right(D4);
Servo s1;
Servo s2;
HttpServer http(80);

void dump(String name, String value) {
  Serial.println("name=" + name + "; value=" + value);
}

void handleRoot(HttpRequest &request, HttpResponse &response) {
  Serial.println("Root path handler");
  Serial.println("HEADER 'Host': " + request.getHeaders()->get("Host"));
  request.getHeaders()->forEach(dump);
  Serial.println("QUERY STRING PARAM 'test': " + request.getQueryStringParams()->get("test"));
  request.getQueryStringParams()->forEach(dump);
  response.status(200, "In root handler!");
  response.header("Content-Type", "text/plain");
  response.beginBody();
  response.println("Hello, world! from root handler :)");
  response.endBody();
}

void handlePostToRoot(HttpRequest &request, HttpResponse &response) {
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

  http.registerHandler("GET", "/", handleRoot);
  http.registerHandler("POST", "/", handlePostToRoot);
  http.begin();
}

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

  http.run();
  // network.run();
}
