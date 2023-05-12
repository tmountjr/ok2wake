#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>

// Include config.
#include "secrets.h"

// Set up global variables.
#include "globals.h"

// Include LED information
#include "led.h"

// Include web server functionality.
#include "WebServer.h"

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "Please define WIFI_PASSWORD in src/secrets.h"
#endif

#ifndef WIFI_SSID
#define WIFI_SSID "Please define WIFI_SSID in src/secrets.h"
#endif

void setup()
{
  Serial.begin(115200);

  setUpLed();

  Serial.print("\nStarting wifi");
  WiFi.hostname("ok2wake");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println(" Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");

  // Set up filesystem.
  LittleFS.begin();

  // Read settings.
  readSettings();

  // Set up WiFi server
  wifiServerSetup();

  ArduinoOTA.begin();

  timeClient.begin();

  // Retrieve the settings from flash and use the timezone name that was last saved.
  // Default to "America/New_York" (set above) if there's a problem.
  File settingsFile = LittleFS.open("settings.json", "r");
  if (!settingsFile)
  {
    Serial.println("Unable to open 'settings.json' from flash. Using defaults.");
  }
  else
  {
    StaticJsonDocument<512> settingsDoc;
    DeserializationError e = deserializeJson(settingsDoc, settingsFile);
    if (e)
    {
      Serial.println("Unable to deserialize 'settings.json' from flash. Using defaults.");
      Serial.println(e.c_str());
    }
    else
    {
      tz_name = settingsDoc["tz_name"].as<String>();
    }
  }
  updateTimeClient();

#if defined FIREBASE_DATABASE_URL && defined FIREBASE_DATABASE_SECRET
  // String payload = "{\"event\":\"power_on\",\"ip\":\"" + WiFi.localIP().toString() + "\"}";
  // updateFirebaseLog(payload);
#endif

  // A sane set of defaults has been provided as part of the firmware in events.json.
  File defaultEventList = LittleFS.open(eventsPath, "r");
  StaticJsonDocument<512> doc;
  DeserializationError e = deserializeJson(doc, defaultEventList);
  if (e)
  {
    Serial.println("Failed to read user config.");
    // Put in a placeholder that keeps the thing turned off.
    LEDEvent *event = new LEDEvent(0, 0, LEDEvent::LED_STATE_OFF);
    ll.add(event);
  }
  else
  {
    for (JsonObject item : doc["events"].as<JsonArray>())
    {
      int hour = item["hour"];
      int minute = item["minute"];
      int state = item["state"];
      LEDEvent *event = new LEDEvent(hour, minute, state);
      ll.add(event);
    }
  }
  findCurrent(ll);
  targetLedStatus = current->data->state;
}

void loop()
{
  if (nextUp->data->hour == timeClient.getHours() && nextUp->data->minute == timeClient.getMinutes())
  {
    current = current->next;
    nextUp = current->next;
    targetLedStatus = current->data->state;
  }

  switch (targetLedStatus)
  {
  case LEDEvent::LED_STATE_WAKE:
    wake();
    break;
  case LEDEvent::LED_STATE_SLEEP:
    sleep();
    break;
  case LEDEvent::LED_STATE_NIGHT_LIGHT:
    night_light();
    break;
  case LEDEvent::LED_STATE_CUSTOM:
    custom();
    break;
  case LEDEvent::LED_STATE_OFF:
  default:
    off();
    break;
  }

  ArduinoOTA.handle();
  server.handleClient();
}