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

// Define RGB LED behavior on wake.
void wake()
{
  int color[] = {0, 255, 0};
  RGB_A->immediate(color);
  RGB_B->immediate(color);
}

// Define RGB LED behavior on sleep.
void sleep()
{
  int color[] = {210, 0, 255};
  RGB_A->immediate(color);
  RGB_B->immediate(color);
}

// Define RGB LED behavior when turned off.
void off()
{
  int color[] = {0, 0, 0};
  RGB_A->immediate(color);
  RGB_B->immediate(color);
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