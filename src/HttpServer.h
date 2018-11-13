#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <Arduino.h>
#include <WiFiServer.h>
#include "HttpRequest.h"
#include "HttpResponse.h"

typedef void (*HttpRequestPathHandler)(HttpRequest &request, HttpResponse &response);

class HttpServer {
  public:
    HttpServer(uint16_t port, String defaultPage = "index.html");
    ~HttpServer();
    void begin();
    void on(String method, String path, HttpRequestPathHandler handler);
    void run();
  private:
    WiFiServer *server;
    String defaultPage;
    LinkedList<HttpRequestPathHandler> handlers;
};

#endif
