#pragma once

#include <Arduino.h>

#define GAUGE_MAX_VALUE 255

class AnalogWheel {
    public:
        void init(uint8_t, uint8_t);
        void set(int);

    private:
        uint8_t motorPinA, motorPinB;
};