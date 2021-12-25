#include "analoggauge.h"

#if defined(ESP32)
const int gagueChannel = 0;
#endif

void AnalogGauge::init(uint8_t _pin)
{
    pin = _pin;
    pinMode(pin, OUTPUT);
#if defined(ESP32)
    ledcSetup(gagueChannel, 1000, 8);
    ledcAttachPin(pin, gagueChannel);
#endif
}

void AnalogGauge::set(uint8_t _value)
{
    uint8_t value = map(_value, 0, 255, 0, GAUGE_MAX_VALUE);

#if defined(ESP32)
    ledcWrite(gagueChannel, value);
#else
    analogWrite(pin, value);
#endif
}