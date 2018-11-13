#include <FS.h>
#include "HttpServer.h"
#include "MimeType.h"

HttpServer::HttpServer(uint16_t port) {
  this->server = new WiFiServer(port);
}

HttpServer::~HttpServer() {
  this->server->stop();
  delete this->server;
}

void HttpServer::begin() {
  server->begin();
  Serial.print("SPIFFS.begin() = ");
  Serial.println(SPIFFS.begin());
}

void HttpServer::registerHandler(String method, String path, PathHandler handler) {
  handlers.append(method + " " + path, handler);
}

void respondWithFile(String path, HttpResponse &response) {
  byte buffer[256];
  File f = SPIFFS.open(path, "r");
  response.status(200, "FILE");
  response.header("Content-Type", getMimeType(path));
  response.header("Content-Size", String(f.size()));
  response.beginBody();
  while (f.position() < f.size()) {
    int bytesRead = f.read(buffer, 256);
    response.write(buffer, bytesRead);
  }
  f.close();
  response.endBody();
}

void respondWith404(HttpRequest &request, HttpResponse &response) {
  response.status(404, "NOT FOUND");
  response.header("Content-Type", "text/plain");
  response.beginBody();
  response.printf("%s %s not found\n", request.getMethod().c_str(), request.getPath().c_str());
}

void defaultRequestHandler(HttpRequest &request, HttpResponse &response) {
  String path = request.getPath() == "/" ? "/index.html" : request.getPath();
  if (SPIFFS.exists(path)) {
    respondWithFile(path, response);
  } else {
    respondWith404(request, response);
  }
}

void HttpServer::run() {
  WiFiClient client = server->available();
  if (client) {
    HttpRequest request(client);
    PathHandler handler = handlers.get(request.getMethod() + " " + request.getPath(), defaultRequestHandler);
    HttpResponse response(client);
    handler(request, response);
  }
}
