#include "DimmableLED.h"

DimmableLED::DimmableLED() {
  this->_setup = false;
}

DimmableLED::DimmableLED(int pin)
{
  this->_pin = pin;
  this->_setup = false;
}

int DimmableLED::_valueAtTick(unsigned long currentTick, unsigned long startTick)
{
  // Handle edge cases.
  if (currentTick < startTick)
    return this->startValue;
  if (currentTick > startTick + this->fadeDuration)
    return this->endValue;

  double percentBrightness = ((double)currentTick - (double)startTick) / (double)this->fadeDuration;
  int toReturn = (percentBrightness * (double)this->_valueDistance) + (double)this->startValue;

  // Handle up and down separately.
  if (this->endValue > this->startValue)
  {
    if (toReturn > this->endValue)
      toReturn = this->endValue;
    if (toReturn < this->startValue)
      toReturn = this->startValue;
  }
  else if (this->startValue > this->endValue)
  {
    if (toReturn < this->endValue)
      toReturn = this->endValue;
    if (toReturn > this->startValue)
      toReturn = this->startValue;
  }

  return toReturn;
}

void DimmableLED::fade(int startValue, int endValue, unsigned long fadeDuration)
{
  unsigned long currentTick = millis();

#ifdef DEBUG
  Serial.printf("Current tick: %lu\n", currentTick);
#endif
  if (!this->_setup)
  {
    this->startValue = startValue;
    this->endValue = endValue;
    this->fadeDuration = fadeDuration;
    this->_startTime = currentTick;
    this->_valueDistance = endValue - startValue;
    this->_setup = true;
    this->complete = false;

    // Serial.printf("Setup complete! Going from %d to %d in %lu ticks.\n", this->startValue, this->endValue, this->fadeDuration);
  }

  if (this->currentValue != this->endValue)
  {
#ifdef DEBUG
    Serial.printf("Current tick: %lu; start time: %lu\n", currentTick, this->_startTime);
#endif

    this->currentValue = this->_valueAtTick(currentTick, this->_startTime);

#ifdef DEBUG
    Serial.printf("Percent brightness: %.2f\n", percentBrightness);
    Serial.printf("Setting led to brightness %d\n\n", this->currentValue);
#endif
  }
  else
  {
    this->complete = true;
#ifdef DEBUG
    Serial.println("Done!");
#endif
  }

  analogWrite(this->_pin, this->currentValue);
}

void DimmableLED::immediate(int value) {
  this->currentValue = value;
  analogWrite(this->_pin, value);
  this->complete = true;
}

void DimmableLED::reset()
{
  this->startValue = -1;
  this->endValue = -1;
  this->fadeDuration = -1;
  this->currentValue = -1;
  this->_valueDistance = 0;
  this->complete = false;
  this->_setup = false;
}