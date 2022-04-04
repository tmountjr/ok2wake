#include <Arduino.h>

// RGB LED pins
#define RGB_A_B D1
#define RGB_A_G D2
#define RGB_A_R D3

#define RGB_B_B D5
#define RGB_B_G D6
#define RGB_B_R D7

// Define RGB LED behavior on wake.
void wake()
{
  analogWrite(RGB_A_R, 0);
  analogWrite(RGB_A_G, 255);
  analogWrite(RGB_A_B, 0);
  analogWrite(RGB_B_R, 0);
  analogWrite(RGB_B_G, 255);
  analogWrite(RGB_B_B, 0);
}

// Define RGB LED behavior on sleep.
void sleep()
{
  analogWrite(RGB_A_R, 210);
  analogWrite(RGB_A_G, 0);
  analogWrite(RGB_A_B, 255);
  analogWrite(RGB_B_R, 210);
  analogWrite(RGB_B_G, 0);
  analogWrite(RGB_B_B, 255);
}

// Define RGB LED behavior when turned off.
void off()
{
  digitalWrite(RGB_A_R, LOW);
  digitalWrite(RGB_A_G, LOW);
  digitalWrite(RGB_A_B, LOW);
  digitalWrite(RGB_B_R, LOW);
  digitalWrite(RGB_B_G, LOW);
  digitalWrite(RGB_B_B, LOW);
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