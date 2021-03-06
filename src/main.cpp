#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>

// Include config.
#include "secrets.h"

// Set up global variables.
#include "globals.h"

// Include web server functionality.
#include "WebServer.h"

// Include LED information
#include "led.h"

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "Please define WIFI_PASSWORD in src/secrets.h"
#endif

#ifndef WIFI_SSID
#define WIFI_SSID "Please define WIFI_SSID in src/secrets.h"
#endif

// EDT offset from GMT in seconds
#define EDT_OFFSET -14400L

void setup()
{
  Serial.begin(115200);

  setUpLed();

  Serial.print("\nStarting wifi");
  WiFi.hostname("ok2wake");
  // WiFi.config(WIFI_STATIC_IP, WIFI_GATEWAY, WIFI_SUBNET);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println(" Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");

  // Set up filesystem.
  LittleFS.begin();

  // Set up WiFi server
  wifiServerSetup();

  ArduinoOTA.begin();

  timeClient.begin();
  unsigned long current_time = 0;
  Serial.print("Getting time");
  while (current_time < 1000)
  {
    Serial.print(".");
    timeClient.update();
    current_time = timeClient.getEpochTime();
    delay(500);
  }
  timeClient.setTimeOffset(EDT_OFFSET);
  Serial.println(" Done!");
  Serial.printf("Started running at %s\n\n", timeClient.getFormattedTime());

  // A sane set of defaults has been provided as part of the firmware in events.json.
  File defaultEventList = LittleFS.open(eventsPath, "r");
  StaticJsonDocument<512> doc;
  DeserializationError e = deserializeJson(doc, defaultEventList);
  if (e)
  {
    Serial.println("Failed to read user config.");
    // Put in a placeholder that keeps the thing turned off.
    LEDEvent *event = new LEDEvent(0, 0, LEDEvent::LED_STATE_OFF);
    ll.add(event);
  }
  else
  {
    for (JsonObject item : doc["events"].as<JsonArray>())
    {
      int hour = item["hour"];
      int minute = item["minute"];
      int state = item["state"];
      LEDEvent *event = new LEDEvent(hour, minute, state);
      ll.add(event);
    }
  }
  findCurrent(ll);
  targetLedStatus = current->data->state;
}

void loop()
{
  if (nextUp->data->hour == timeClient.getHours() && nextUp->data->minute == timeClient.getMinutes())
  {
    current = current->next;
    nextUp = current->next;
    targetLedStatus = current->data->state;
  }

  switch (targetLedStatus)
  {
  case LEDEvent::LED_STATE_WAKE:
    wake();
    break;
  case LEDEvent::LED_STATE_SLEEP:
    sleep();
    break;
  case LEDEvent::LED_STATE_OFF:
  default:
    off();
    break;
  }

  ArduinoOTA.handle();
  server.handleClient();
}