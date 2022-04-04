#include "DimmableRGBLED.h"

DimmableRGBLED::DimmableRGBLED(int redPin, int greenPin, int bluePin)
{
  this->_redPin = redPin;
  this->_greenPin = greenPin;
  this->_bluePin = bluePin;

  this->_red = DimmableLED(redPin);
  this->_green = DimmableLED(greenPin);
  this->_blue = DimmableLED(bluePin);
}

int *DimmableRGBLED::_valueAtTick(unsigned long currentTick, unsigned long startTick)
{
  int r = this->_red._valueAtTick(currentTick, startTick);
  int g = this->_green._valueAtTick(currentTick, startTick);
  int b = this->_blue._valueAtTick(currentTick, startTick);
  int toReturn[] = {r, g, b};
  return toReturn;
}

void DimmableRGBLED::fade(int startValue[], int endValue[], unsigned long fadeDuration)
{
  int r_start = startValue[0];
  int g_start = startValue[1];
  int b_start = startValue[2];
  int r_end = endValue[0];
  int g_end = endValue[1];
  int b_end = endValue[2];

  this->_red.fade(r_start, r_end, fadeDuration);
  this->_green.fade(g_start, g_end, fadeDuration);
  this->_blue.fade(b_start, b_end, fadeDuration);

  this->complete = this->_red.complete && this->_blue.complete && this->_green.complete;
}

void DimmableRGBLED::immediate(int value[]) {
  int r_value = value[0];
  int g_value = value[1];
  int b_value = value[2];
  this->_red.immediate(r_value);
  this->_green.immediate(g_value);
  this->_blue.immediate(b_value);

  this->complete = this->_red.complete && this->_blue.complete && this->_green.complete;
}

void DimmableRGBLED::reset() {
  this->_red.reset();
  this->_green.reset();
  this->_blue.reset();
}