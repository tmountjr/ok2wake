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
    Node<LEDEvent> *c = current;

    DynamicJsonDocument jsonObject(1024);
    String jsonObjectString;
    JsonArray events = jsonObject.createNestedArray("events");

    JsonObject currentObject = events.createNestedObject();
    currentObject["hour"] = c->data->hour;
    currentObject["minute"] = c->data->minute;
    currentObject["state"] = c->data->state;

    while (c->next != current)
    {
      currentObject = events.createNestedObject();
      currentObject["hour"] = c->next->data->hour;
      currentObject["minute"] = c->next->data->minute;
      currentObject["state"] = c->next->data->state;
      c = c->next;
    }

    serializeJson(jsonObject, jsonObjectString);
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
    DynamicJsonDocument jsonObject(1024);
    jsonObject["t"] = timeClient.getEpochTime();
    jsonObject["s"] = targetLedStatus;
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
        if (newState == LEDEvent::LED_STATE_OFF || newState == LEDEvent::LED_STATE_SLEEP || newState == LEDEvent::LED_STATE_WAKE)
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

void wifiServerSetup()
{
  server.on("/events", getEventsResponse);
  server.on("/status/set", setLedStatusResponse);
  server.on("/status", getStatusResponse);
  server.on("/reset", resetResponse);
  server.on("/", getHomeResponse);
  server.serveStatic("/main.js", LittleFS, "main.js");

  server.onNotFound(notFoundResponse);
  server.begin();
}