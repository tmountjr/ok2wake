#include <Arduino.h>
#include <stdexcept>

class LEDEvent
{
public:
  LEDEvent();
  LEDEvent(int hour, int minute, int state);

  /**
   * Determine if this event is to take place after a specified hour and minute.
   * @param targetHour    The hour to compare against (0-23).
   * @param targetMinute  The minute to compare against (0-59).
   */
  bool isFuture(int targetHour, int targetMinute);

  /**
   * Determine if this event is to take place now given a specified hour and minute.
   * @param targetHour    The hour to compare against (0-23).
   * @param targetMinute  The minute to compare against (0-59).
   */
  bool isCurrent(int targetHour, int targetMinute);

  /**
   * Determine if this event is to take place before a specified hour and minute.
   * @param targetHour    The hour to compare against (0-23).
   * @param targetMinute  The minute to compare against (0-59).
   */
  bool isPast(int targeHour, int targetMinute);

  int hour, minute, state;

  static const int LED_STATE_WAKE = 1;
  static const int LED_STATE_SLEEP = 2;
  static const int LED_STATE_OFF = 3;
  static const int LED_STATE_NIGHT_LIGHT = 4;
  static const int LED_STATE_CUSTOM = 5;
};

LEDEvent::LEDEvent() {
  this->hour = 0;
  this->minute = 0;
  this->state = this->LED_STATE_OFF;
}

LEDEvent::LEDEvent(int hour, int minute, int state) {
  this->hour = hour;
  this->minute = minute;
  this->state = state;
}

bool LEDEvent::isFuture(int targetHour, int targetMinute) {
  if (targetHour < 0 || targetHour > 23 || targetMinute < 0 || targetMinute > 59) {
    throw new std::out_of_range("Hours must be between 0 and 23; minutes must be between 0 and 59.");
  }

  return (
    this->hour > targetHour ||
    (this->hour == targetHour && this->minute >= targetMinute)
  );
}

bool LEDEvent::isPast(int targetHour, int targetMinute) {
  return !this->isFuture(targetHour, targetMinute);
}

bool LEDEvent::isCurrent(int targetHour, int targetMinute) {
  if (targetHour < 0 || targetHour > 23 || targetMinute < 0 || targetMinute > 59) {
    throw new std::out_of_range("Hours must be between 0 and 23; minutes must be between 0 and 59.");
  }

  return !(this->isFuture(targetHour, targetMinute) || this->isPast(targetHour, targetMinute));
}
