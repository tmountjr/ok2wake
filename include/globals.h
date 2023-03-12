#pragma once

#include <Arduino.h>
#include <LEDEvent.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <LinkedList.h>
#include <string>
#include <ArduinoJson.h>
#include <LittleFS.h>

// NTP setup
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Web server setup
ESP8266WebServer server(8080);
HTTPClient http;
WiFiClient client;

signed long tz_offset = 0L;
String tz_name = "America/New_York";

void updateTimeClient()
{
  unsigned long current_time = 0;
  Serial.print("Getting time and TZ offset for ");
  Serial.print(tz_name);
  while (current_time < 1000)
  {
    Serial.print(".");
    timeClient.update();
    current_time = timeClient.getEpochTime();
    delay(500);
  }


  if (WiFi.status() == WL_CONNECTED)
  {
    if (http.begin(client, "http://worldtimeapi.org/api/timezone/" + tz_name))
    {
      int httpCode = http.GET();
      if (httpCode == 200)
      {
        DynamicJsonDocument apiResp(2048);
        String resp = http.getString();
        DeserializationError e = deserializeJson(apiResp, resp);
        if (e)
        {
          Serial.println("Failed to read timezone information");
          Serial.println(e.c_str());
        }
        else
        {
          tz_offset = apiResp["raw_offset"].as<signed long>();
          bool is_dst = apiResp["dst"].as<bool>();
          if (is_dst)
          {
            signed int dst_offset = apiResp["dst_offset"].as<signed int>();
            tz_offset = tz_offset + dst_offset;
          }
        }
      }
      http.end();
    }
  }

  timeClient.setTimeOffset(tz_offset);
  Serial.println(" Done!");
  Serial.printf("Calculated time: %s\n\n", timeClient.getFormattedTime().c_str());
}

// Target LED status (allows for overrides)
byte targetLedStatus;

// Linked List setup
LinkedList<LEDEvent> ll;
Node<LEDEvent> *current, *nextUp;

String jsonPath = "events.json";
const char *eventsPath = jsonPath.c_str();

// Find the event that is next to fire.
void findCurrent(LinkedList<LEDEvent> l)
{
  current = l.head;
  nextUp = current->next;

  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();

  // Edge case: if the first event is future or the last event is past, don't loop
  // because in both cases we use the head of the list.
  if (
      l.tail->data->isPast(currentHour, currentMinute) ||
      l.head->data->isFuture(currentHour, currentMinute))
  {
    current = l.tail;
    nextUp = current->next;
    return;
  }

  // Set the pointers to where current is in the past and next is in the future.
  while (!(current->data->isPast(currentHour, currentMinute) && current->next->data->isFuture(currentHour, currentMinute)))
  {
    current = current->next;
    nextUp = current->next;
  }
}

// Read settings from the filesystem.
bool readSettings()
{
  File settingsFile = LittleFS.open("settings.json", "r");
  if (!settingsFile)
  {
    Serial.println("Unable to open 'settings.json' from flash. Using defaults.");
    return false;
  }

  StaticJsonDocument<512> settingsDoc;
  DeserializationError e = deserializeJson(settingsDoc, settingsFile);
  if (e)
  {
    Serial.println("Unable to deserialize 'settings.json' from flash. Using defaults.");
    return false;
  }

  tz_name = settingsDoc["tz_name"].as<String>();
  return true;
}

// Write settings to the filesystem.
bool writeSettings()
{
  LittleFS.remove("settings.json");
  File settingsFile = LittleFS.open("settings.json", "w");
  if (!settingsFile)
  {
    Serial.println("Unable to create 'settings.json'.");
    return false;
  }

  StaticJsonDocument<512> settingsDoc;
  settingsDoc["tz_name"] = tz_name;
  if (serializeJson(settingsDoc, settingsFile) == 0)
  {
    Serial.println("Unable to write to 'settings.json'.");
    return false;
  }
  Serial.println("Write new settings to 'settings.json'.");
  return true;
}
