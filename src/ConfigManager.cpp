#include <FS.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include "ConfigManager.h"
#include "AccessPoint.h"

ConfigMgr ConfigManager;

void ConfigMgr::start(String hostname, IPAddress ip, String(*getConfig)(), bool(*setConfig)(String)) {
  ESP8266WebServer web(80);
  DNSServer dns;
  bool done = false;

  // Needed to serve static content of the captive portal
  SPIFFS.begin();

  // Start the access point of the captive portal
  AccessPoint.start(hostname, ip);

  // DNS server is needed so that captive portal knows it is not connected to the internet
  dns.setErrorReplyCode(DNSReplyCode::NoError);
  dns.start(53, "*", WiFi.softAPIP());

  // Configure web server to respond to configuration URLs
  web.on("/hotspot-detect.html", HTTP_GET, [&web]() {
    web.sendHeader("Location", String("/cp/"), true);
    web.send(302, "text/plain", "Redirecting to captive portal");
  });

  // Configuration get/set API
  web.on("/api/config", HTTP_GET, [&web, getConfig]() {
    web.send(200, "application/text", getConfig());
  });
  web.on("/api/config", HTTP_POST, [&web, setConfig, &done]() {
    if (setConfig(web.arg("plain"))) {
      web.send(200, "text/plain", "Settings saved");
      done = true;
    } else {
      web.send(500, "text/plain", "Settings not saved");
    }
  });
  // Serve static content
  web.serveStatic("/cp/", SPIFFS, "/cp/");
  web.begin();

  while (!done) {
    dns.processNextRequest();
    web.handleClient();
  }

  web.stop();
  dns.stop();
  AccessPoint.stop();
}
