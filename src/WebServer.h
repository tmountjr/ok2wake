/**
 * The web server functionality has been abstracted here as much as possible.
 * Routes are defined at the end of the file once all the handlers have been defined.
 * All other global variables should come from globals.h.
 */

#include "globals.h"

void notFoundResponse()
{
  server.send(404, "text/plain", "Not Found");
}

void invalidRequestResponse(int status = 400, String msg = "Bad Request")
{
  server.send(status, "text/plain", msg);
}

void statusResponse()
{
  HTTPMethod method = server.method();
  if (method == HTTP_HEAD)
  {
    server.send(200, "application/json", "");
  }
  else if (method == HTTP_GET)
  {
    char buff[1024];
    sprintf(buff, "{\"t\":%lu,\"s\":%d}\r\n", timeClient.getEpochTime(), current.ledstate);
    server.send(200, "application/json", buff);
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
  for (int i = 0; i < server.args(); i++) {
    if (server.argName(i) == "ledstate") {
      int newState = server.arg(i).toInt();
      if (newState == LEDEvent::LED_STATE_OFF || newState == LEDEvent::LED_STATE_SLEEP || newState == LEDEvent::LED_STATE_WAKE) {
        targetLedStatus = newState;
        server.send(200, "text/plain", "OK");
      }
      break;
    }
  }
  invalidRequestResponse();
}

void resetResponse() {
  HTTPMethod method = server.method();
  if (!(method == HTTP_POST || method == HTTP_PUT)) {
    invalidRequestResponse(405, "Method Not Allowed");
  }
  targetLedStatus = current.ledstate;
  server.send(200, "text/plain", "OK");
}

void wifiServerSetup()
{
  server.on("/status/set", setLedStatusResponse);
  server.on("/status", statusResponse);
  server.on("/reset", resetResponse);

  server.onNotFound(notFoundResponse);
  server.begin();
}