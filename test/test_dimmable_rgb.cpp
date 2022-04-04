#include <Arduino.h>
#include <DimmableRGBLED.h>
#include <unity.h>

DimmableRGBLED led(5, 4, 0);

void process() {
  UNITY_BEGIN();
  // Tests go here.
}

byte finished = false;
int red[] = {255, 0, 0};
int green[] = {0, 255, 0};

void setup() {
  delay(2000);
  process();
  led.reset();
}

void loop() {
  if (!finished) {
    led.fade(red, green, 5000L);
    finished = led.complete;
  } else {
    // RUN_TEST(test_complete_flag);
    UNITY_END();
  }
}