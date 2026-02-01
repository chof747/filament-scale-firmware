#include <Arduino.h>

#include "config.h"

const float calibrationFactors[MAX_SCALES] = {411450.0f, 411450.0f, 411450.0f};
const int doutPins[MAX_SCALES] = {DOUT1, DOUT2, DOUT3};
const int clkPins[MAX_SCALES] = {CLK0, CLK1, CLK2};
