#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <WiFiClient.h>
#include "LinkedList.h"

class HttpRequestHeaders : public LinkedList<String> {
  public:
    String get(String name, String def = "") {
      return LinkedList::get(name, def);
    }
};

class HttpRequestQueryStringParams : public LinkedList<String> {
  public:
    String get(String name, String def = "") {
      return LinkedList::get(name, def);
    }
};

class HttpRequest {
  public:
    HttpRequest(WiFiClient &client, String defaultPage);
    IPAddress getOrigin();
    String getMethod();
    String getPath();
    String getDefaultPage();
    String getProtocol();
    String getHeader(String name, String def = "");
    HttpRequestHeaders *getHeaders();
    String getQueryStringParam(String name, String def = "");
    HttpRequestQueryStringParams *getQueryStringParams();
    String getBody();
  private:
    WiFiClient *client;
    String method;
    String path;
    String defaultPage;
    String protocol;
    HttpRequestHeaders headers;
    HttpRequestQueryStringParams params;
    String body;
    void parse();
    void parseStatusLine();
    void parseHeaders();
    void parseQueryStringParams();
    void parseBody();
};

#endif
