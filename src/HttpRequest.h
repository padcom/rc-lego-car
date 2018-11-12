#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <WiFiClient.h>
#include "LinkedList.h"

class HttpRequestHeaders : public LinkedList<String> {
  public:
    String get(String name) {
      return LinkedList::get(name, "");
    }
};

class HttpRequestQueryStringParams : public LinkedList<String> {
  public:
    String get(String name) {
      return LinkedList::get(name, "");
    }
};

class HttpRequest {
  public:
    HttpRequest(WiFiClient &client);
    IPAddress getOrigin();
    String getMethod();
    String getPath();
    String getProtocol();
    String getHeader(String name);
    HttpRequestHeaders *getHeaders();
    String getQueryStringParam(String name);
    HttpRequestQueryStringParams *getQueryStringParams();
  private:
    WiFiClient *client;
    String method;
    String path;
    String protocol;
    HttpRequestHeaders headers;
    HttpRequestQueryStringParams params;
    void parse();
    void parseStatusLine();
    void parseHeaders();
    void parseQueryStringParams();
};

#endif
