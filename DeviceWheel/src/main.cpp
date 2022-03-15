
/*
    Three WiFi Meters - Device Wheel
    -
    Originally written by David Chatting - github.com/davidchatting/ThreeWiFiMeters
    Updated by Andriy Malyshenko - https://github.com/anabolyc/esp32-esp8266-wifi-meaters
    MIT License - Copyright (c) March 2021
    Documented here > https://github.com/davidchatting/ThreeWiFiMeters#-device-wheel
*/

#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#endif
#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif

#include <Approximate.h>
Approximate approx;

#ifdef WHEEL_TFT
#include "tftwheel.h"
TFTWheel *tftwheel = new TFTWheel();
#define TFT_WHEEL_SET(value) tftwheel->set(value);
#else
#define TFT_WHEEL_SET(value) ;
#endif

#ifdef WHEEL_ANALOG
#include "analogwheel.h"
AnalogWheel *awheel = new AnalogWheel();
#define A_WHEEL_SET(value) awheel->set(value);
#else
#define A_WHEEL_SET(value) ;
#endif

#define WHEEL_SET(value)  \
  {                       \
    TFT_WHEEL_SET(value); \
    A_WHEEL_SET(value);   \
  }

int targetMotorSpeed = 0;
const unsigned int motorUpdateIntervalMs = 250;
long nextMotorUpdateAtMs = 0;

bool newPair = false;

bool blink(int periodMs)
{
  return (((millis() / periodMs) % 2) == 0);
}

void setPair(const char *macAddress)
{
  Serial.printf("Paired with: %s\n", macAddress);
  approx.setActiveDeviceFilter(macAddress);
}

void onProximateDevice(Device *device, Approximate::DeviceEvent event)
{
  switch (event)
  {
  case Approximate::ARRIVE:
    newPair = true;
    char macAdddress[18];
    device->getMacAddressAs_c_str(macAdddress);
    //(*settings)["pair"] = macAdddress;
    //settings->save();
    setPair(macAdddress);
    Serial.println("ARRIVE\t" + device->getMacAddressAsString());
    break;
  case Approximate::DEPART:
    Serial.println("DEPART\t" + device->getMacAddressAsString());
    break;
  }
}

void setTargetMotorSpeed(int v)
{
  targetMotorSpeed = constrain(v, -1024, 1024);
}

void onActiveDevice(Device *device, Approximate::DeviceEvent event)
{
  int payloadSizeByte = device->getPayloadSizeBytes();
  if (device->isDownloading())
    payloadSizeByte *= -1;

  setTargetMotorSpeed(payloadSizeByte);
  Serial.printf("onActiveDevice: %d\n", payloadSizeByte);
}

void updateMotorSpeed()
{
  if (millis() > nextMotorUpdateAtMs)
  {
    WHEEL_SET(targetMotorSpeed);

    targetMotorSpeed = 0;
    nextMotorUpdateAtMs = millis() + motorUpdateIntervalMs;
  }
}

void setup()
{
  Serial.begin(SERIAL_BAUD);

#ifdef WHEEL_TFT
  tftwheel->init();
#endif
#ifdef WHEEL_ANALOG
  awheel->init(WHEEL_ANALOG_PIN_A, WHEEL_ANALOG_PIN_B);
#endif

  // WiFi.begin(WIFI_SSID, WIFI_PASS);
  // Serial.printf("Connecting to %s\n", WIFI_SSID);

  // while (WiFi.status() != WL_CONNECTED)
  // {
  //   delay(400);
  //   Serial.print(".");
  // }
  // Serial.print(" connected!");

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  if (approx.init(WIFI_SSID, WIFI_PASS))
  {
    approx.setProximateDeviceHandler(onProximateDevice, APPROXIMATE_INTIMATE_RSSI, /*lastSeenTimeoutMs*/ 3000);
    approx.setActiveDeviceHandler(onActiveDevice, /*inclusive*/ false);
    approx.begin();
  }

  WHEEL_SET(1024);
  delay(25);
  WHEEL_SET(0);
}

void loop()
{
  approx.loop();

  // uint8_t wifiStatus = WiFi.status();
  // Serial.printf("Wifi status: %d\n", wifiStatus);

  if (approx.isRunning())
  {
    if (!newPair)
      digitalWrite(PIN_LED, HIGH);
    else
    {
      digitalWrite(PIN_LED, LOW);
      delay(200);
      digitalWrite(PIN_LED, HIGH);
      newPair = false;
    }
  }
  else
  {
    WHEEL_SET(0);
    digitalWrite(PIN_LED, blink(1000));
  }

  updateMotorSpeed();
  // delay(1000);
}