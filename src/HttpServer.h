#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <Arduino.h>
#include <WiFiServer.h>
#include "HttpRequest.h"
#include "HttpResponse.h"

typedef void (*PathHandler)(HttpRequest &request, HttpResponse &response);

class HttpServer {
  public:
    HttpServer(uint16_t port, String defaultPage = "index.html");
    ~HttpServer();
    void begin();
    void registerHandler(String method, String path, PathHandler handler);
    void run();
  private:
    WiFiServer *server;
    String defaultPage;
    LinkedList<PathHandler> handlers;
};

#endif
