class LEDEvent {
  static LED_STATE_WAKE = 1;
  static LED_STATE_SLEEP = 2;
  static LED_STATE_OFF = 3;
  static LED_STATE_NIGHT_LIGHT = 4;
  static LED_STATE_CUSTOM = 5;

  constructor(hour, minute, state) {
    this.hour = hour;
    this.minute = minute;
    this.state = state;
  }

  isFuture(targetHour, targetMinute) {
    this.validateInput(targetHour, targetMinute);
    return (
      this.hour > targetHour ||
      (this.hour == targetHour && this.minute >= targetMinute)
    );
  }

  isCurrent(targetHour, targetMinute) {
    this.validateInput(targetHour, targetMinute);
    return this.hour == targetHour && this.minute == targetMinute;
  }

  isPast(targetHour, targetMinute) {
    return !(this.isFuture(targetHour, targetMinute) || this.isCurrent(targetHour, targetMinute));
  }

  validateInput(targetHour, targetMinute) {
    if (targetHour < 0 || targetHour > 23 || targetMinute < 0 || targetMinute > 59) {
      throw new Error('Hours must be between 0 and 23; minutes must be between 0 and 59.');
    }
  }

  secondsSinceMidnight() {
    return (this.hour * 60 * 60) + (this.minute * 60);
  }

  toJSON() {
    return {
      hour: this.hour,
      minute: this.minute,
      state: this.state
    };
  }
}

module.exports = {
  LEDEvent
};