#include "HttpResponse.h"

HttpResponse::HttpResponse(WiFiClient &client) {
  this->client = &client;
}

void HttpResponse::status(int status, String message) {
  if (statusSent) {
    Serial.println("ERROR: status has already been sent!");
    return;
  }
  client->printf("HTTP/1.1 %d %s\n", status, message.c_str());
  statusSent = true;
}

void HttpResponse::header(String name, String value) {
  if (!statusSent) {
    Serial.println("ERROR: status has not been sent yet!");
    return;
  }
  if (headersSent) {
    Serial.println("ERROR: cannot sent header when sending response body!");
    return;
  }
  client->print(name);
  client->print(": ");
  client->println(value);
}

void HttpResponse::beginBody() {
  headersSent = true;
  client->println();
}

void HttpResponse::endBody() {
  client->println();
  client->println();
  bodySent = true;
}

size_t HttpResponse::write(uint8_t byte) {
  if (bodySent) {
    Serial.println("ERROR: body has already been sent!");
    return -1;
  }
  return client->write(byte);
}

size_t HttpResponse::write(const uint8_t *buffer, size_t size) {
  if (bodySent) {
    Serial.println("ERROR: body has already been sent!");
    return -1;
  }
  return client->write(buffer, size);
}

void HttpResponse::flush() {
  client->flush();
}
