#include <Arduino.h>
#include <DimmableLED.h>

#define BUILTIN D0
DimmableLED dimmable(BUILTIN);
long fade_duration = 5000L; // five seconds

void setup() {
  pinMode(BUILTIN, OUTPUT);

  // Remember that the builtin LED is reversed, LOW = on
  digitalWrite(BUILTIN, LOW);
}

void loop() {
  // This will fade the LED off.
  dimmable.fade(0, 255, fade_duration);
}