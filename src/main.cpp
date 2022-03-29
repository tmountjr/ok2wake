#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <secrets.h>

#ifndef WIFI_PASSWORD
  #define WIFI_PASSWORD "Please define WIFI_PASSWORD in src/secrets.h"
#endif

// RGB LED pins
#define RGB_A_B D1
#define RGB_A_G D2
#define RGB_A_R D3

#define RGB_B_B D5
#define RGB_B_G D6
#define RGB_B_R D7

#define FLIP_INTERVAL 43200000L // 12h * 60m * 60s * 1000ms
// #define FLIP_INTERVAL 5000L // 5 seconds for testing purposes

// NTP setup
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// TODO: set up flash memory storage of the "ok time" and the "sleep time"

// TODO: set up a simple web app to manage the following options:
//  1. "ok time" color (RGB)
//  2. "sleep time" color (RGB)
//  3. some sort of schedule, allowing multiple windows for "ok" and "sleep"

unsigned long millis_until_next = -1;
int target_hours[] = {7, 19};
int target_minutes[] = {0, 0};
byte wake[] = {1, 0};
byte wake_state = 1;

void flip() {
  // Serial.print("Flipping: old state = "); Serial.print(wake_state); Serial.print("; new state = "); Serial.print(!wake_state);
  wake_state = !wake_state;
}

void green() {
  analogWrite(RGB_A_R, 0);
  analogWrite(RGB_A_G, 255);
  analogWrite(RGB_A_B, 0);
  analogWrite(RGB_B_R, 0);
  analogWrite(RGB_B_G, 255);
  analogWrite(RGB_B_B, 0);
}

void purple() {
  analogWrite(RGB_A_R, 210);
  analogWrite(RGB_A_G, 0);
  analogWrite(RGB_A_B, 255);
  analogWrite(RGB_B_R, 210);
  analogWrite(RGB_B_G, 0);
  analogWrite(RGB_B_B, 255);
}

void setup() {
  Serial.begin(115200);

  pinMode(RGB_A_R, OUTPUT);
  pinMode(RGB_A_G, OUTPUT);
  pinMode(RGB_A_R, OUTPUT);
  pinMode(RGB_B_B, OUTPUT);
  pinMode(RGB_B_G, OUTPUT);
  pinMode(RGB_B_B, OUTPUT);
  digitalWrite(RGB_A_R, LOW);
  digitalWrite(RGB_A_G, LOW);
  digitalWrite(RGB_A_B, LOW);
  digitalWrite(RGB_B_R, LOW);
  digitalWrite(RGB_B_G, LOW);
  digitalWrite(RGB_B_B, LOW);

  const char *ssid = "ihatecomputers";

  WiFi.begin(ssid, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  ArduinoOTA.begin();

  Serial.println("\n");
  Serial.println("Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  timeClient.begin();
  timeClient.update();
  unsigned long current_time = timeClient.getEpochTime();
  Serial.print("Getting time");
  while (current_time < 1000) {
    Serial.print(".");
    timeClient.update();
    current_time = timeClient.getEpochTime();
    delay(500);
  }
  Serial.println("\n");

  Serial.print("Started running at ");
  Serial.println(timeClient.getFormattedTime());

  // Calculate the time between now and whatever the next event is
  int target_index = 0;
  bool found_target = false;

  int current_hour = timeClient.getHours() - 4;
  if (current_hour < 0) current_hour += 24;
  int current_minute = timeClient.getMinutes();

  // Serial.println("*** current time ***");
  // Serial.print("current time: "); Serial.print(current_hour);
  // Serial.print(":"); Serial.println(current_minute);

  int target_length = sizeof(target_hours) / sizeof(target_hours[0]);
  for (int i = 0; i < target_length; i += 1) {
    Serial.print("target time: "); Serial.print(target_hours[i]); Serial.print(":"); Serial.println(target_minutes[i]);
    if (target_hours[i] > current_hour) {
      Serial.println("   target_hours < current_hour");
      target_index = i;
      found_target = true;
    } else if (target_hours[i] == current_hour) {
      Serial.println("   target_hours == current_hour");
      // Check that we're note in the right hour but still waiting
      if (target_minutes[i] > current_minute) {
        Serial.println("      target_minutes > current_minute");
        target_index = i;
        found_target = true;
      }
    }
    if (found_target) break;
  }

  Serial.println("After iteration:");
  Serial.print("target index: "); Serial.println(target_index);
  Serial.print("found? ");
  if (found_target) {
    Serial.println("yes");
  } else {
    Serial.println("no");
  }

  // wake_state should be the state in target_index - 1
  // if that is a negative number, it becomes the last element
  // if that matches the sizeof, it becomes 0
  Serial.print("Target index: "); Serial.println(target_index);
  int wake_state_index = target_index - 1;
  if (wake_state_index < 0) wake_state_index = target_length - 1;
  if (wake_state_index == target_length) wake_state_index = 0;
  Serial.print("Wake state index: "); Serial.println(wake_state_index);
  wake_state = wake[wake_state_index];

  int hours_until_next = target_hours[target_index] - current_hour;
  if (!found_target) hours_until_next = (24 - current_hour) + target_hours[0];
  if (hours_until_next > 0) hours_until_next -= 1;
  
  int minutes_until_next = hours_until_next * 60;
  minutes_until_next += target_minutes[target_index] - current_minute;

  millis_until_next = minutes_until_next * 60 * 1000;
  // Serial.println("Until next event:");
  // Serial.print("Hours: ");
  // Serial.println(hours_until_next);
  // Serial.print("Minutes: ");
  // Serial.println(minutes_until_next);

  // Serial.print("millis() until next event: ");
  // Serial.println(millis_until_next);
}

void loop() {
  unsigned long current_millis = millis();
  if (millis_until_next > 0 && current_millis >= millis_until_next) {
    flip();
    millis_until_next = current_millis + FLIP_INTERVAL;
  }
  if (wake_state) {
    green();
  } else {
    purple();
  }

  ArduinoOTA.handle();
}