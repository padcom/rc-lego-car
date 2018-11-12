#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <WiFiClient.h>

class HttpResponse : public Print {
  public:
    HttpResponse(WiFiClient &client);
    void status(int status, String message);
    void header(String name, String value);
    void beginBody();
    void endBody();
    size_t write(uint8_t byte);
    size_t write(const uint8_t *buffer, size_t size);
    void flush();
  private:
    WiFiClient *client;
    bool statusSent = false;
    bool headersSent = false;
    bool bodySent = false;
};

#endif
