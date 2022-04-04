#pragma once

#include <Arduino.h>
#include <LEDEvent.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <ESP8266WebServer.h>
#include <LinkedList.h>
#include <string>

// NTP setup
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Web server setup
ESP8266WebServer server(8080);

// Target LED status (allows for overrides)
byte targetLedStatus;

// Linked List setup
LinkedList<LEDEvent> ll;
Node<LEDEvent> *current, *nextUp;

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