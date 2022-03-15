#include "tftwheel.h"

void TFTWheel::init()
{
    tft = new TFT_eSPI();
    tft->init();
    tft->setRotation(TFT_ROTATION);

    arr_center_x = tft->width() >> 1;
    arr_center_y = (tft->height() * 3) >> 2;
    arc_size_x = arr_center_x;
    arc_size_y = arr_center_y - 10;
    prev_value = 0;
    text_pos_x = (tft->width() >> 1) - 10;
    text_pos_y = (tft->height() >> 1) - 20;

    // this->grawBackground(true, true, true, true, true);
}

void TFTWheel::grawBackground(
    bool grawBg,
    bool drawArc,
    bool drawNumbers,
    bool drawArcPins,
    bool drawCaption)
{
    if (grawBg)
    {
        // fill background
        tft->fillScreen(COLOR_BG);
    }

    if (drawNumbers || drawCaption)
    {
        tft->setTextColor(COLOR_ARC_LOW, COLOR_BG);
    }

    if (drawNumbers || drawArcPins)
    {
        for (int16_t pos = RSSI_MIN, ang = ANGLE_START - 90; pos <= RSSI_MAX; pos += RSSI_STEP, ang += ANGLE_STEP)
        {
            // Calculate first pair of coordinates for first num
            float sx = cos(ang * DEG2RAD);
            float sy = sin(ang * DEG2RAD);
            if (drawArcPins)
            {
                // draw scale points
                uint16_t x0 = sx * arc_size_x + arr_center_x;
                uint16_t y0 = sy * arc_size_y + arr_center_y;
                uint16_t x1 = sx * arc_size_x * 1.05 + arr_center_x;
                uint16_t y1 = sy * arc_size_y * 1.05 + arr_center_y;
                tft->drawLine(x0, y0, x1, y1, pos == RSSI_MAX ? COLOR_ARC_HIGH : COLOR_ARC_LOW);
            }

            if (drawNumbers)
            {
                // draw numbers
                uint16_t x = sx * arc_size_x * 0.93 + arr_center_x - 10;
                uint16_t y = sy * arc_size_y * 0.93 + arr_center_y;
                tft->drawNumber(pos, x, y, 2);
            }
        }
    }

    if (drawArc)
    {
        // draw scale
        tft->fillArc(arr_center_x, arr_center_y,
                     ANGLE_START, (ANGLE_TOTAL - ANGLE_STEP) / 6,
                     arc_size_x, arc_size_y,
                     4, COLOR_ARC_LOW);

        tft->fillArc(arr_center_x, arr_center_y,
                     ANGLE_END - ANGLE_STEP, ANGLE_STEP / 6,
                     arc_size_x, arc_size_y,
                     4, COLOR_ARC_HIGH);
    }

    if (drawCaption)
    {
        // draw RSSI
        tft->drawString("RSSI", arr_center_x - 30, arr_center_y + 20, 4);
    }
}

void TFTWheel::drawArrow(int16_t value, uint32_t color)
{
    int16_t ang = map(value, RSSI_MIN, RSSI_MAX, ANGLE_START, ANGLE_END) - 90;
    float sx = cos(ang * DEG2RAD);
    float sy = sin(ang * DEG2RAD);

    uint16_t x0 = sx * arc_size_x * 0.95 + arr_center_x;
    uint16_t y0 = sy * arc_size_y * 0.95 + arr_center_y;
    uint16_t x1 = -sx * arc_size_x * 0.1 + arr_center_x;
    uint16_t y1 = -sy * arc_size_y * 0.1 + arr_center_y;
    tft->drawLine(x0, y0, x1, y1, color);

    // center of the arrow
    tft->drawCircle(arr_center_x, arr_center_y, 15, color);
    tft->fillCircle(arr_center_x, arr_center_y, 5, color);
}

void TFTWheel::drawValue(int16_t value, uint32_t color)
{
    tft->setTextColor(color);
    tft->setCursor(text_pos_x, text_pos_y, 2);
    tft->print(value);
    // tft->print(" Db");
}

void TFTWheel::set(int16_t value)
{
    if (value == prev_value)
        return;

    Serial.printf("RSSI: %d\n", value);

    drawValue(prev_value, COLOR_BG);
    // drawArrow(prev_value, COLOR_BG);

    drawValue(value, COLOR_ARROW);
    // drawArrow(value, COLOR_ARROW);

    // this->grawBackground(false, true, true, false, false);

    prev_value = value;
}