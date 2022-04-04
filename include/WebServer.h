/**
 * The web server functionality has been abstracted here as much as possible.
 * Routes are defined at the end of the file once all the handlers have been defined.
 * All other global variables should come from globals.h.
 */

#include "globals.h"
#include <ArduinoJson.h>

void notFoundResponse()
{
  server.send(404, "text/plain", "Not Found");
}

void invalidRequestResponse(int status = 400, String msg = "Bad Request")
{
  server.send(status, "text/plain", msg);
}

void getEventsResponse()
{
  HTTPMethod method = server.method();
  if (!(method == HTTP_GET || method == HTTP_HEAD))
  {
    invalidRequestResponse(405, "Method Not Allowed");
  }
  if (method == HTTP_HEAD)
  {
    server.send(200, "application/json", "");
  }

  Node<LEDEvent> *c = current;

  DynamicJsonDocument jsonObject(1024);
  String jsonObjectString;
  JsonArray events = jsonObject.createNestedArray("events");

  JsonObject currentObject = events.createNestedObject();
  currentObject["hour"] = c->data->hour;
  currentObject["minute"] = c->data->minute;
  currentObject["state"] = c->data->ledstate;

  while (c->next != current)
  {
    currentObject = events.createNestedObject();
    currentObject["hour"] = c->next->data->hour;
    currentObject["minute"] = c->next->data->minute;
    currentObject["state"] = c->next->data->ledstate;
    c = c->next;
  }

  serializeJson(jsonObject, jsonObjectString);
  server.send(200, "application/json", jsonObjectString);
}

void getStatusResponse()
{
  HTTPMethod method = server.method();
  if (method == HTTP_HEAD)
  {
    server.send(200, "application/json", "");
  }
  else if (method == HTTP_GET)
  {
    DynamicJsonDocument jsonObject(1024);
    jsonObject["t"] = timeClient.getEpochTime();
    jsonObject["s"] = targetLedStatus;
    String jsonObjectString;
    serializeJson(jsonObject, jsonObjectString);
    server.send(200, "application/json", jsonObjectString);
  }
  else
  {
    invalidRequestResponse(405, "Method Not Allowed");
  }
}

void setLedStatusResponse()
{
  HTTPMethod method = server.method();
  if (!(method == HTTP_POST || method == HTTP_PUT))
  {
    invalidRequestResponse(405, "Method Not Allowed");
  }
  // Go through the input and look for "ledstate"
  for (int i = 0; i < server.args(); i++)
  {
    if (server.argName(i) == "ledstate")
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

void resetResponse()
{
  HTTPMethod method = server.method();
  if (!(method == HTTP_POST || method == HTTP_PUT))
  {
    invalidRequestResponse(405, "Method Not Allowed");
  }
  targetLedStatus = current->data->ledstate;
  server.send(200, "text/plain", "OK");
}

void wifiServerSetup()
{
  server.on("/events", getEventsResponse);
  server.on("/status/set", setLedStatusResponse);
  server.on("/status", getStatusResponse);
  server.on("/reset", resetResponse);

  server.onNotFound(notFoundResponse);
  server.begin();
}