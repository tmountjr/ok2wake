#include <Arduino.h>
#include <ESP8266HTTPClient.h>

#include "Weblog.h"

int Weblog::post(String data)
{
  WiFiClient client;
  HTTPClient http;

  http.begin(client, this->_endpointUrl);
  http.addHeader("Content-Type", "application/json");

  // combine the data with the message template
  String msg = "{\"message\":\"" + data;
  msg += "\"}";

  int httpCode = http.POST(msg);
  http.end();
  return httpCode;
}