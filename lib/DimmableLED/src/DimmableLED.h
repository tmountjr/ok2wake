#include <Arduino.h>

// Create a dimmable LED that fades between two values in a specific time.
class DimmableLED
{
protected:
  // The millis() that the fade started.
  unsigned long _startTime;

  // The distance between the start and end brightness values.
  signed int _valueDistance;

  // The pin that the LED lives on.
  int _pin;

  // Whether or not the fade has been set up.
  byte _setup = false;

public:
  DimmableLED();
  DimmableLED(int pin);

  // The starting brightness value.
  int startValue;

  // The ending brightness value.
  int endValue;

  // The current brightness value.
  int currentValue = -1;

  // The fade duration.
  unsigned long fadeDuration;

  // Whether or not the fade has completed.
  byte complete = false;

  /**
   * Determine the value of this LED at any given tick.
   *
   * @param currentTick The tick for which to calculate the value.
   * @param startTick   The tick at which the fade began.
   */
  int _valueAtTick(unsigned long currentTick, unsigned long startTick);

  /**
   * Fade an LED.
   *
   * @param startValue   The starting brightness.
   * @param endValue     The ending brightness.
   * @param fadeDuration The length of time for the fade.
   */
  void fade(int startValue, int endValue, unsigned long fadeDuration);

  /**
   * Set an LED to a specific value immediately.
   * 
   * @param value The value to set on the LED.
   */
  void immediate(int value);

  // Reset the details on this LED.
  void reset();
};
