#include "DimmableLED.h"

class RGBValues
{
public:
  RGBValues(int r, int g, int b);

  int red, green, blue;
};

class DimmableRGBLED
{
protected:
  int _redPin;
  int _greenPin;
  int _bluePin;
  DimmableLED _red;
  DimmableLED _green;
  DimmableLED _blue;

public:
  DimmableRGBLED(int redPin, int greenPin, int bluePin);
  RGBValues _valueAtTick(unsigned long currentTick, unsigned long startTick);
  void fade(RGBValues startValue, RGBValues endValue, unsigned long fadeDuration);
  void immediate(RGBValues value);
  void reset();

  byte complete = false;
};
