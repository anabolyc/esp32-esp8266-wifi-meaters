#pragma once

#include <Arduino.h>

#define GAUGE_MAX_VALUE 255

class AnalogGauge {
    public:
        void init(uint8_t);
        void set(uint8_t);

    private:
        uint8_t pin;
};