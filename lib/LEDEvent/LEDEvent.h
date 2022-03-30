#include <Arduino.h>

class LEDEvent
{
public:
  LEDEvent();
  LEDEvent(int hour, int minute, byte ledstate);
  int hour, minute;
  byte ledstate;
};

LEDEvent::LEDEvent() {
  this->hour = 0;
  this->minute = 0;
  this->ledstate = 0;
}

LEDEvent::LEDEvent(int hour, int minute, byte ledstate) {
  this->hour = hour;
  this->minute = minute;
  this->ledstate = ledstate;
}