#pragma once

#include <Arduino.h>

void onceConnected();
void displayRSSI();
int32_t getRSSI(String target_ssid);
