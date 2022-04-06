#include "DimmableRGBLED.h"

RGBValues::RGBValues(int r, int g, int b)
{
  this->red = r;
  this->green = g;
  this->blue = b;
}

DimmableRGBLED::DimmableRGBLED(int redPin, int greenPin, int bluePin)
{
  this->_redPin = redPin;
  this->_greenPin = greenPin;
  this->_bluePin = bluePin;

  this->_red = DimmableLED(redPin);
  this->_green = DimmableLED(greenPin);
  this->_blue = DimmableLED(bluePin);
}

RGBValues DimmableRGBLED::_valueAtTick(unsigned long currentTick, unsigned long startTick)
{
  RGBValues toReturn(
    this->_red._valueAtTick(currentTick, startTick),
    this->_green._valueAtTick(currentTick, startTick),
    this->_blue._valueAtTick(currentTick, startTick)
  );
  return toReturn;
}

void DimmableRGBLED::fade(RGBValues startValue, RGBValues endValue, unsigned long fadeDuration)
{
  int r_start = startValue.red;
  int g_start = startValue.green;
  int b_start = startValue.blue;
  int r_end = endValue.red;
  int g_end = endValue.green;
  int b_end = endValue.blue;

  this->_red.fade(r_start, r_end, fadeDuration);
  this->_green.fade(g_start, g_end, fadeDuration);
  this->_blue.fade(b_start, b_end, fadeDuration);

  this->complete = this->_red.complete() && this->_blue.complete() && this->_green.complete();
}

void DimmableRGBLED::immediate(RGBValues value)
{
  int r_value = value.red;
  int g_value = value.green;
  int b_value = value.blue;
  this->_red.immediate(r_value);
  this->_green.immediate(g_value);
  this->_blue.immediate(b_value);

  this->complete = this->_red.complete() && this->_blue.complete() && this->_green.complete();
}

void DimmableRGBLED::reset() {
  this->_red.reset();
  this->_green.reset();
  this->_blue.reset();
}