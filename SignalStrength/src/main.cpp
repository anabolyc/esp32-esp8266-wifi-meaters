/*
    Three WiFi Meters - Signal Strength
    -
    Originally written by David Chatting - github.com/davidchatting/ThreeWiFiMeters
    Updated by Andriy Malyshenko - https://github.com/anabolyc/esp32-esp8266-wifi-meaters
    MIT License - Copyright (c) March 2021
    Documented here > https://github.com/davidchatting/ThreeWiFiMeters#-signal-strength
*/

#include <Arduino.h>

#ifdef ESP32
#include <WiFi.h>
#endif
#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif

#ifdef GAUGE_TFT
#include "tftgauge.h"
TFTGauge *tftgauge = new TFTGauge();
#define TFT_GAUGE_SET(value) tftgauge->set(value);
#else
#define TFT_GAUGE_SET(value) ;
#endif

#ifdef GAUGE_ANALOG
#include "analoggauge.h"
AnalogGauge *agauge = new AnalogGauge();
#define A_GAUGE_SET(value) agauge->set(value);
#else
#define A_GAUGE_SET(value) ;
#endif

#define GAUGE_SET(value)  \
  {                       \
    TFT_GAUGE_SET(value); \
    A_GAUGE_SET(value);   \
  }

bool blink(int periodMs)
{
  return (((millis() / periodMs) % 2) == 0);
}

// Return RSSI or 0 if target SSID not found
int32_t getRSSI(String target_ssid)
{
  int32_t result = 0;

  byte available_networks = WiFi.scanNetworks();

  for (int network = 0; network < available_networks && result == 0; network++)
  {
    if (WiFi.SSID(network).equals(target_ssid))
    {
      result = WiFi.RSSI(network);
    }
  }
  return (result);
}

void displayRSSI()
{
  int32_t rssi = getRSSI(WiFi.SSID());
  if (rssi != 0)
    GAUGE_SET(rssi);
}

void setup()
{
  Serial.begin(SERIAL_BAUD);

#ifdef GAUGE_TFT
  tftgauge->init();
#endif
#ifdef GAUGE_ANALOG
  agauge->init(GAUGE_ANALOG);
#endif

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.printf("Connecting to %s\n", WIFI_SSID);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(400);
    Serial.print(".");
  }
  Serial.print(" connected!");

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  // GAUGE_SET(255);
  delay(150);
  GAUGE_SET(-100);
}

void loop()
{
  uint8_t wifiStatus = WiFi.status();
  Serial.printf("Wifi status: %d\n", wifiStatus);

  if (wifiStatus == WL_CONNECTED)
  {
    digitalWrite(PIN_LED, HIGH);
    displayRSSI();
  }
  else
  {
    GAUGE_SET(-100);
    digitalWrite(PIN_LED, blink(1000));
  }

  delay(1000);
}