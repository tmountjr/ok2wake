#pragma once

#include <Arduino.h>
#include <LEDEvent.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <ESP8266WebServer.h>
#include <string>

// NTP setup
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// LED class
LEDEvent current, next;

// Web server setup
ESP8266WebServer server(8080);

// Target LED status (allows for overrides)
byte targetLedStatus;