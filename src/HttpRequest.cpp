#include "HttpRequest.h"

HttpRequest::HttpRequest(WiFiClient &client) {
  this->client = &client;
  parse();
}

IPAddress HttpRequest::getOrigin() {
  return this->client->remoteIP();
}

String HttpRequest::getMethod() {
  return this->method;
}

String HttpRequest::getPath() {
  return this->path;
}

String HttpRequest::getProtocol() {
  return this->protocol;
}

String HttpRequest::getHeader(String name) {
  return this->headers.get(name);
}

HttpRequestHeaders *HttpRequest::getHeaders() {
  return &this->headers;
}

String HttpRequest::getQueryStringParam(String name) {
  return this->params.get(name);
}

HttpRequestQueryStringParams *HttpRequest::getQueryStringParams() {
  return &this->params;
}

void HttpRequest::parse() {
  parseStatusLine();
  parseHeaders();
  parseQueryStringParams();
}

void HttpRequest::parseStatusLine() {
  String request = client->readStringUntil('\r');
  Serial.println("Request: " + request);

  int methodDelimiter = request.indexOf(' ');
  int protocolDelimiter = request.lastIndexOf(' ');

  Serial.print("methodDelimiter: ");
  Serial.println(methodDelimiter);
  Serial.print("protocolDelimiter: ");
  Serial.println(protocolDelimiter);

  method = request.substring(0, methodDelimiter);
  Serial.println("method: '" + method + "'");

  path = request.substring(methodDelimiter + 1, protocolDelimiter);
  Serial.println("path: '" + path + "'");

  protocol = request.substring(protocolDelimiter + 1, request.length());
  Serial.println("protocol: '" + protocol + "'");
}

void HttpRequest::parseHeaders() {
  do {
    String header = client->readStringUntil('\r');
    Serial.println("HEADER: '" + header + "'");
    if (header.startsWith("\n")) {
      Serial.println("Header starts with a new line - trimming...");
      header.remove(0, 1);
      Serial.println("HEADER: '" + header + "'");
    }
    int delimiter = header.indexOf(": ");
    if (delimiter >= 0) {
      String name = header.substring(0, delimiter);
      String value = header.substring(delimiter + 2, header.length());
      Serial.println("Header '" + name + "'='" + value + "'");
      headers.append(name, value);
    } else {
      break;
    }
  } while(true);
}

void HttpRequest::parseQueryStringParams() {
  int index = path.indexOf("?");
  if (index == -1) {
    Serial.println("No query string parameters found");
    return;
  }
  String all = path.substring(index + 1);
  Serial.println("All query string parameters: " + all);
  path.remove(index);
  Serial.println("Path after removal of query string parameters: " + path);
  do {
    index = all.indexOf("&");
    String param;
    if (index >= 0) {
      param = all.substring(0, index);
      Serial.println("param after extraction: " + param);
      all.remove(0, index + 1);
      Serial.println("all after extraction: " + all);
    } else {
      param = all;
      all = "";
    }
    index = param.indexOf('=');
    String name;
    String value;
    if (index >= 0) {
      name = param.substring(0, index);
      Serial.println("Param name: " + name);
      value = param.substring(index + 1, param.length());
      Serial.println("Param value: " + value);
    } else {
      name = param;
      value = "";
    }
    params.append(name, value);
  } while (all.length() > 0);
}
