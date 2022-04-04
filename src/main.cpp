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

  LEDEvent *am = new LEDEvent(7, 0, LEDEvent::LED_STATE_WAKE);
  LEDEvent *midday = new LEDEvent(8, 0, LEDEvent::LED_STATE_OFF);
  LEDEvent *pm = new LEDEvent(18, 30, LEDEvent::LED_STATE_SLEEP);
  LEDEvent *overnight = new LEDEvent(19, 0, LEDEvent::LED_STATE_OFF);
  ll.add(am);
  ll.add(midday);
  ll.add(pm);
  ll.add(overnight);
  findCurrent(ll);
  targetLedStatus = current->data->ledstate;
}

void loop()
{
  if (nextUp->data->hour == timeClient.getHours() && nextUp->data->minute == timeClient.getMinutes())
  {
    current = current->next;
    nextUp = current->next;
    targetLedStatus = current->data->ledstate;
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