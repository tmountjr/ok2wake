#include "DimmableLED.h"

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
  int * _valueAtTick(unsigned long currentTick, unsigned long startTick);
  void fade(int startValue[], int endValue[], unsigned long fadeDuration);
  void immediate(int value[]);
  void reset();

  byte complete = false;
};
