#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>

#define GAUGE_MAX_VALUE 255
#define COLOR_BG TFT_YELLOW
#define COLOR_ARC_LOW TFT_BLUE
#define COLOR_ARC_HIGH TFT_RED
#define COLOR_ARROW TFT_BLACK

#define RSSI_MIN -100
#define RSSI_MAX 0
#define RSSI_STEP 20
#define RSSI_STEPS ((RSSI_MAX - RSSI_MIN) / RSSI_STEP)

#define ANGLE_START -60
#define ANGLE_END 60
#define ANGLE_TOTAL (ANGLE_END - ANGLE_START)
#define ANGLE_STEP (ANGLE_TOTAL / RSSI_STEPS)

class TFTWheel
{
public:
    void init();
    void set(int16_t);

private:
    TFT_eSPI *tft;
    uint16_t arr_center_x, arr_center_y;
    uint16_t arc_size_x, arc_size_y;
    uint16_t text_pos_x, text_pos_y;
    int16_t prev_value;

    void grawBackground(bool, bool, bool, bool, bool);
    void drawArrow(int16_t, uint32_t);
    void drawValue(int16_t, uint32_t);
};