#include "HttpServer.h"

HttpServer::HttpServer(uint16_t port) {
  this->server = new WiFiServer(port);
}

HttpServer::~HttpServer() {
  Serial.println("HttpServer::~HttpServer");
  delete this->server;
}

void HttpServer::begin() {
  server->begin();
}

void HttpServer::registerHandler(String method, String path, PathHandler handler) {
  handlers.append(method + " " + path, handler);
}

void defaultErrorHandler(HttpRequest &request, HttpResponse &response) {
  response.status(404, "NOT FOUND");
  response.header("Content-Type", "text/plain");
  response.beginBody();
  response.printf("%s %s not found", request.getMethod().c_str(), request.getPath().c_str());
}

void HttpServer::run() {
  WiFiClient client = server->available();
  if (client) {
    HttpRequest request(client);
    PathHandler handler = handlers.get(request.getMethod() + " " + request.getPath(), defaultErrorHandler);
    HttpResponse response(client);
    handler(request, response);
  }
}
