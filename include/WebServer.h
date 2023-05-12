/**
 * The web server functionality has been abstracted here as much as possible.
 * Routes are defined at the end of the file once all the handlers have been defined.
 * All other global variables should come from globals.h.
 */

#include "globals.h"
#include <ArduinoJson.h>
#include <LittleFS.h>

void corsResponse()
{
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(204, "text/plain", "");
  
}

void notFoundResponse()
{
  server.send(404, "text/plain", "Not Found");
}

void invalidRequestResponse(int status = 400, String msg = "Bad Request")
{
  server.send(status, "text/plain", msg);
}

void getHomeResponse()
{
  HTTPMethod method = server.method();
  if (!(method == HTTP_GET || method == HTTP_HEAD))
    invalidRequestResponse(405, "Method Not Allowed");
  else
  {
    File index = LittleFS.open("index.html", "r");
    String indexStr;
    if (index.available())
    {
      indexStr = index.readString();
      server.send(200, "text/html", indexStr);
    }
    else
      server.send(503, "text/plain", "Service Unavailable");
  }
}

void setTimeZoneResponse()
{
  HTTPMethod method = server.method();
  if (!(method == HTTP_POST || method == HTTP_OPTIONS))
    invalidRequestResponse(405, "Method Not Allowed");

  if (method == HTTP_OPTIONS)
    corsResponse();
  else
  {
    StaticJsonDocument<512> inputDoc;
    deserializeJson(inputDoc, server.arg("plain"));
    tz_name = inputDoc["tz_name"].as<String>();
    updateTimeClient();
    StaticJsonDocument<96> jsonObject;
    jsonObject["t"] = timeClient.getEpochTime();
    jsonObject["s"] = targetLedStatus;
    jsonObject["o"] = tz_offset;
    jsonObject["tz"] = tz_name;
    String jsonObjectString;
    serializeJson(jsonObject, jsonObjectString);
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", jsonObjectString);

    // Write the new value back to flash memory.
    writeSettings();
  }
}

void setEventsResponse()
{
  HTTPMethod method = server.method();
  if (!(method == HTTP_POST || method == HTTP_OPTIONS))
    invalidRequestResponse(405, "Method Not Allowed");

  if (method == HTTP_OPTIONS)
    corsResponse();
  else
  {
    StaticJsonDocument<512> inputDoc;
    deserializeJson(inputDoc, server.arg("plain"));
    LinkedList<LEDEvent> newLL;
    for (JsonObject item : inputDoc.as<JsonArray>())
    {
      int hour = item["hour"];
      int minute = item["minute"];
      int state = item["state"];
      LEDEvent *e = new LEDEvent(hour, minute, state);
      newLL.add(e);
    }
    ll = newLL;
    findCurrent(ll);
    
    LittleFS.remove(eventsPath);
    File file = LittleFS.open(eventsPath, "w");
    if (!file)
    {
      Serial.println("Error creating user config file.");
      server.send(500, "text/plain", "Cannot create user config file; the config has been saved in temporary flash storage.");
    }
    bool seenHead = false;
    Node<LEDEvent> *c = ll.head;
    StaticJsonDocument<512> outputDoc;
    JsonArray events = outputDoc.createNestedArray("events");
    while (!seenHead) {
      JsonObject o = events.createNestedObject();
      o["hour"] = c->data->hour;
      o["minute"] = c->data->minute;
      o["state"] = c->data->state;
      c = c->next;
      if (c == ll.head)
        seenHead = true;
    }
    if (serializeJson(outputDoc, file) == 0)
    {
      Serial.println("Error writing new user config file.");
      server.send(500, "text/plain", "Error saving new config file; the config has been saved in temporary flash storage.");
    }

    server.send(200, "text/plain");
  }
}

void getEventsResponse()
{
  HTTPMethod method = server.method();
  if (!(method == HTTP_GET || method == HTTP_HEAD || method == HTTP_OPTIONS))
    invalidRequestResponse(405, "Method Not Allowed");

  if (method == HTTP_HEAD)
    server.send(200, "application/json", "");
  else if (method == HTTP_OPTIONS)
    corsResponse();
  else
  {
    bool seenCurrent = false;
    Node<LEDEvent> *c = current;
    StaticJsonDocument<512> doc;
    JsonArray events = doc.createNestedArray("events");
    while (!seenCurrent) {
      JsonObject o = events.createNestedObject();
      o["hour"] = c->data->hour;
      o["minute"] = c->data->minute;
      o["state"] = c->data->state;
      c = c->next;
      if (c == current)
        seenCurrent = true;
    }
    String jsonObjectString;
    serializeJson(doc, jsonObjectString);
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", jsonObjectString);
  }
}

void getStatusResponse()
{
  HTTPMethod method = server.method();
  if (method == HTTP_HEAD)
    server.send(200, "application/json", "");
  else if (method == HTTP_OPTIONS)
    corsResponse();
  else if (method == HTTP_GET)
  {
    StaticJsonDocument<96> jsonObject;
    jsonObject["t"] = timeClient.getEpochTime();
    jsonObject["s"] = targetLedStatus;
    jsonObject["o"] = tz_offset;
    jsonObject["tz"] = tz_name;
    String jsonObjectString;
    serializeJson(jsonObject, jsonObjectString);
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", jsonObjectString);
  }
  else
    invalidRequestResponse(405, "Method Not Allowed");
}

void setLedStatusResponse()
{
  HTTPMethod method = server.method();
  if (!(method == HTTP_POST || method == HTTP_PUT))
    invalidRequestResponse(405, "Method Not Allowed");
  else if (method == HTTP_OPTIONS)
    corsResponse();
  else
  {
    // Go through the input and look for "state"
    for (int i = 0; i < server.args(); i++)
    {
      if (server.argName(i) == "state")
      {
        int newState = server.arg(i).toInt();
        if (
          newState == LEDEvent::LED_STATE_OFF ||
          newState == LEDEvent::LED_STATE_SLEEP ||
          newState == LEDEvent::LED_STATE_WAKE ||
          newState == LEDEvent::LED_STATE_NIGHT_LIGHT ||
          newState == LEDEvent::LED_STATE_CUSTOM
        )
        {
          targetLedStatus = newState;
          server.send(200, "text/plain", "OK");
        }
        break;
      }
    }
    invalidRequestResponse();
  }
}

void resetResponse()
{
  HTTPMethod method = server.method();
  if (!(method == HTTP_POST || method == HTTP_PUT))
    invalidRequestResponse(405, "Method Not Allowed");
  else if (method == HTTP_OPTIONS)
    corsResponse();
  else {
    targetLedStatus = current->data->state;
    server.send(200, "text/plain", "OK");
  }
}

void setColorResponse()
{
  HTTPMethod method = server.method();
  if (!(method == HTTP_POST || method == HTTP_OPTIONS))
    invalidRequestResponse(405, "Method Not Allowed");

  if (method == HTTP_OPTIONS)
    corsResponse();
  else
  {
    StaticJsonDocument<512> inputDoc;
    deserializeJson(inputDoc, server.arg("plain"));
    int event_id = inputDoc["event_id"].as<int>();
    int red_channel = inputDoc["r"].as<int>();
    int green_channel = inputDoc["g"].as<int>();
    int blue_channel = inputDoc["b"].as<int>();
    // Only support custom for now, until we know it's working
    if (event_id == LEDEvent::LED_STATE_CUSTOM)
    {
      custom_rgb = RGBValues(red_channel, green_channel, blue_channel);
      
      StaticJsonDocument<96> jsonResponseObject;
      jsonResponseObject["t"] = timeClient.getEpochTime();
      jsonResponseObject["s"] = targetLedStatus;
      jsonResponseObject["o"] = tz_offset;
      jsonResponseObject["tz"] = tz_name;
      String jsonObjectString;
      serializeJson(jsonResponseObject, jsonObjectString);
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(200, "application/json", jsonObjectString);
    }
    else {
      invalidRequestResponse();
    }
  }
}

void wifiServerSetup()
{
  server.on("/tz/set", setTimeZoneResponse);
  server.on("/events/set", setEventsResponse);
  server.on("/events", getEventsResponse);
  server.on("/status/set", setLedStatusResponse);
  server.on("/status", getStatusResponse);
  server.on("/reset", resetResponse);
  server.on("/color/set", setColorResponse);
  server.on("/", getHomeResponse);
  server.serveStatic("/main.js", LittleFS, "main.js");

  server.onNotFound(notFoundResponse);
  server.begin();
}