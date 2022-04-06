#include <Arduino.h>
#include <DimmableRGBLED.h>

// RGB LED pins
#define RGB_A_B D1
#define RGB_A_G D2
#define RGB_A_R D3

#define RGB_B_B D5
#define RGB_B_G D6
#define RGB_B_R D7

DimmableRGBLED *RGB_A = new DimmableRGBLED(RGB_A_R, RGB_A_G, RGB_A_B);
DimmableRGBLED *RGB_B = new DimmableRGBLED(RGB_B_R, RGB_B_G, RGB_B_B);

RGBValues wake_rgb(0, 255, 0);
RGBValues sleep_rgb(210, 0, 255);
RGBValues off_rgb(0, 0, 0);

// Define RGB LED behavior on wake.
void wake()
{
  RGB_A->immediate(wake_rgb);
  RGB_B->immediate(wake_rgb);
}

// Define RGB LED behavior on sleep.
void sleep()
{
  RGB_A->immediate(sleep_rgb);
  RGB_B->immediate(sleep_rgb);
}

// Define RGB LED behavior when turned off.
void off()
{
  RGB_A->immediate(off_rgb);
  RGB_B->immediate(off_rgb);
}

// To run in setup() - set the pin modes and turn off.
void setUpLed()
{
  pinMode(RGB_A_R, OUTPUT);
  pinMode(RGB_A_G, OUTPUT);
  pinMode(RGB_A_R, OUTPUT);
  pinMode(RGB_B_B, OUTPUT);
  pinMode(RGB_B_G, OUTPUT);
  pinMode(RGB_B_B, OUTPUT);
  off();
}