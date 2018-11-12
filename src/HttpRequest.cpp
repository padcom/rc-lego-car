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

String HttpRequest::getHeader(String name, String def) {
  return this->headers.get(name, def);
}

HttpRequestHeaders *HttpRequest::getHeaders() {
  return &this->headers;
}

String HttpRequest::getQueryStringParam(String name, String def) {
  return this->params.get(name, def);
}

HttpRequestQueryStringParams *HttpRequest::getQueryStringParams() {
  return &this->params;
}

String HttpRequest::getBody() {
  return body;
}
void HttpRequest::parse() {
  parseStatusLine();
  parseHeaders();
  parseQueryStringParams();
  parseBody();
}

void HttpRequest::parseStatusLine() {
  String request = client->readStringUntil('\r');

  int methodDelimiter = request.indexOf(' ');
  int protocolDelimiter = request.lastIndexOf(' ');

  method = request.substring(0, methodDelimiter);
  path = request.substring(methodDelimiter + 1, protocolDelimiter);
  protocol = request.substring(protocolDelimiter + 1, request.length());
}

void HttpRequest::parseHeaders() {
  do {
    String header = client->readStringUntil('\r');
    if (header.startsWith("\n")) {
      header.remove(0, 1);
    }
    int delimiter = header.indexOf(": ");
    if (delimiter >= 0) {
      String name = header.substring(0, delimiter);
      String value = header.substring(delimiter + 2, header.length());
      headers.append(name, value);
    } else {
      break;
    }
  } while(true);
}

void HttpRequest::parseQueryStringParams() {
  int index = path.indexOf("?");
  if (index == -1) return;
  String all = path.substring(index + 1);
  path.remove(index);
  do {
    index = all.indexOf("&");
    String param;
    if (index >= 0) {
      param = all.substring(0, index);
      all.remove(0, index + 1);
    } else {
      param = all;
      all = "";
    }
    index = param.indexOf('=');
    String name;
    String value;
    if (index >= 0) {
      name = param.substring(0, index);
      value = param.substring(index + 1, param.length());
    } else {
      name = param;
      value = "";
    }
    params.append(name, value);
  } while (all.length() > 0);
}

void HttpRequest::parseBody() {
  client->read();
  int contentLength = atoi(headers.get("Content-Length", "-1").c_str());
  if (contentLength == -1) {
    body = "";
  } else {
    char buffer[contentLength + 1];
    memset(buffer, 0, contentLength + 1);
    client->readBytes(buffer, contentLength);
    body = String(buffer);
  }
}
