#include <Arduino.h>

class LEDEvent
{
public:
  LEDEvent();
  LEDEvent(int hour, int minute, int ledstate);

  int hour, minute, ledstate;

  static const int LED_STATE_WAKE = 1;
  static const int LED_STATE_SLEEP = 2;
  static const int LED_STATE_OFF = 3;
};

LEDEvent::LEDEvent() {
  this->hour = 0;
  this->minute = 0;
  this->ledstate = this->LED_STATE_OFF;
}

LEDEvent::LEDEvent(int hour, int minute, int ledstate) {
  this->hour = hour;
  this->minute = minute;
  this->ledstate = ledstate;
}