#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>

// Include config.
#include "secrets.h"

// Set up global variables.
#include "globals.h"

// Include web server functionality.
#include "WebServer.h"

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "Please define WIFI_PASSWORD in src/secrets.h"
#endif

#ifndef WIFI_SSID
#define WIFI_SSID "Please define WIFI_SSID in src/secrets.h"
#endif

// EDT offset from GMT in seconds
#define EDT_OFFSET -14400L

// RGB LED pins
#define RGB_A_B D1
#define RGB_A_G D2
#define RGB_A_R D3

#define RGB_B_B D5
#define RGB_B_G D6
#define RGB_B_R D7

// Define this here to enable web logging; configure the logging in src/weblog_config.h
// #define WEBLOG
#ifdef WEBLOG
#include <Weblog.h>
Weblog weblog;
#endif

// Rearrange a queue such that the next event to trigger is at the head of the queue.
void orderQueue(ArduinoQueue<LEDEvent> &q)
{
  // cycle through the queue and find the first timestamp that hasn't happened yet
  // assume an ordered list
  for (int i = 0; i < q.count(); i++)
  {
    LEDEvent c = q.peek();
    if (
        c.hour > timeClient.getHours() ||
        (c.hour == timeClient.getHours() && c.minute >= timeClient.getMinutes()))
    {
      // the head of the queue is now the next one to kick off, we're done.
      break;
    }
    q.pop();
    q.push(c);
  }
  // at this point the queue should have the next item at the head of the line,
  // even if it cycled back to the original order.
}

void wake()
{
  analogWrite(RGB_A_R, 0);
  analogWrite(RGB_A_G, 255);
  analogWrite(RGB_A_B, 0);
  analogWrite(RGB_B_R, 0);
  analogWrite(RGB_B_G, 255);
  analogWrite(RGB_B_B, 0);
}

void sleep()
{
  analogWrite(RGB_A_R, 210);
  analogWrite(RGB_A_G, 0);
  analogWrite(RGB_A_B, 255);
  analogWrite(RGB_B_R, 210);
  analogWrite(RGB_B_G, 0);
  analogWrite(RGB_B_B, 255);
}

void off()
{
  digitalWrite(RGB_A_R, LOW);
  digitalWrite(RGB_A_G, LOW);
  digitalWrite(RGB_A_B, LOW);
  digitalWrite(RGB_B_R, LOW);
  digitalWrite(RGB_B_G, LOW);
  digitalWrite(RGB_B_B, LOW);
}

void setup()
{
  Serial.begin(115200);

  pinMode(RGB_A_R, OUTPUT);
  pinMode(RGB_A_G, OUTPUT);
  pinMode(RGB_A_R, OUTPUT);
  pinMode(RGB_B_B, OUTPUT);
  pinMode(RGB_B_G, OUTPUT);
  pinMode(RGB_B_B, OUTPUT);
  off();

  Serial.print("\nStarting wifi");
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

#ifdef WEBLOG
  weblog.post("[setup] Started running at " + timeClient.getFormattedTime());
  delay(3000);
#endif

  LEDEvent am(7, 0, LEDEvent::LED_STATE_WAKE);
  LEDEvent midday(8, 0, LEDEvent::LED_STATE_OFF);
  LEDEvent pm(18, 30, LEDEvent::LED_STATE_SLEEP);
  LEDEvent overnight(19, 0, LEDEvent::LED_STATE_OFF);
  q.push(am);
  q.push(midday);
  q.push(pm);
  q.push(overnight);
  orderQueue(q);
  current = q.last();
  next = q.peek();
  targetLedStatus = current.ledstate;

  Serial.printf("{\"first\":{\"hour\":%d,\"minute\":%d,\"ledstate\":%d},\"last\":{\"hour\":%d,\"minute\":%d,\"ledstate\":%d}}\"\n",
                next.hour,
                next.minute,
                next.ledstate,
                current.hour,
                current.minute,
                current.ledstate);

#ifdef WEBLOG
  LEDEvent next = q.peek();
  String msg = "[setup] Next event time: ";
  msg += next.hour + "h";
  msg += ":";
  msg += next.minute + "m";
  weblog.post(msg);
  delay(3000);
  weblog.post("[setup] Current state: " + targetLedStatus);
  delay(3000);
#endif
}

void loop()
{
  if (next.hour == timeClient.getHours() && next.minute == timeClient.getMinutes())
  {
    current = next;
    q.pop();
    q.push(next);
    next = q.peek();
    targetLedStatus = current.ledstate;
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