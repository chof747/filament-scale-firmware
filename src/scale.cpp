#include "scale.h"

#include <Arduino.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "logging.h"

namespace {
const float kDefaultCalibrationFactors[MAX_SCALES] = {411450.0f, 100000.0f, 10000.0f};
const int kDefaultDoutPins[MAX_SCALES] = {DOUT1, DOUT2, DOUT3};
const int kDefaultClkPins[MAX_SCALES] = {CLK0, CLK1, CLK2};
}

FilamentScale::FilamentScale()
    : FilamentScale(kDefaultCalibrationFactors, kDefaultDoutPins, kDefaultClkPins) {}

FilamentScale::FilamentScale(const float* calibrationFactors,
                             const int* doutPins,
                             const int* clkPins)
    : currentScale_(0),
      displayMode(kDisplayScale) {
  for (int i = 0; i < MAX_SCALES; i++) {
    calibration_factor_[i] = calibrationFactors[i];
    doutPins_[i] = doutPins[i];
    clkPins_[i] = clkPins[i];
  }
}

bool FilamentScale::waitForScaleReady(int index, void (*logFn)(const char*), const char* timeoutMsg) {
  unsigned long startMillis = millis();
  while (!scale_[index].is_ready()) {
    if (millis() - startMillis > TIMEOUT_MS) {
      char message[80];
      snprintf(message, sizeof(message), "Scale %d %s", index, timeoutMsg);
      logFn(message);
      return false;
    }
    delay(10);
  }
  return true;
}

void FilamentScale::setupScale(int index, int doutPin, int clkPin) {
  scale_[index].begin(doutPin, clkPin);

  if (!waitForScaleReady(index, logError, "not ready within timeout. Check wiring!")) {
    return;
  }

  scale_[index].set_scale(calibration_factor_[index]);
  scale_[index].tare(); //Reset the scale to 0
  char message[48];
  snprintf(message, sizeof(message), "Scale %d setup complete.", index);
  logInfo(message);
}

int FilamentScale::getCurrentScale() const {
  return currentScale_;
}

void FilamentScale::setCurrentScale(int index) {
  if (index >= 0 && index < MAX_SCALES) {
    currentScale_ = index;
    displayMode = kDisplayScale;
  }
}

void FilamentScale::advanceScale() {
  if (displayMode == kDisplayScale) {
    if (currentScale_ < MAX_SCALES - 1) {
      currentScale_++;
      return;
    }
    displayMode = kDisplayEnvTemp;
    return;
  }

  if (displayMode == kDisplayEnvTemp) {
    displayMode = kDisplayEnvHum;
    return;
  }

  displayMode = kDisplayScale;
  currentScale_ = 0;
}

FilamentScale::DisplayMode FilamentScale::getDisplayMode() const {
  return displayMode;
}

bool FilamentScale::isDisplayingScale() const {
  return displayMode == kDisplayScale;
}

bool FilamentScale::isDisplayingEnvTemp() const {
  return displayMode == kDisplayEnvTemp;
}

bool FilamentScale::isDisplayingEnvHum() const {
  return displayMode == kDisplayEnvHum;
}

void FilamentScale::formatTemperatureLabel(const EnvReading& reading,
                                           char* out,
                                           size_t size) const {
  if (!reading.isValid) {
    snprintf(out, size, "T: %s", ENV_UNAVAILABLE_TOKEN);
    return;
  }
  char value[10];
  dtostrf(reading.temperatureC, 0, 1, value);
  snprintf(out, size, "T: %sC", value);
}

void FilamentScale::formatHumidityLabel(const EnvReading& reading,
                                        char* out,
                                        size_t size) const {
  if (!reading.isValid) {
    snprintf(out, size, "H: %s", ENV_UNAVAILABLE_TOKEN);
    return;
  }
  char value[10];
  dtostrf(reading.humidityPercent, 0, 1, value);
  snprintf(out, size, "H: %s%%", value);
}

void FilamentScale::setupScales() {
  for (int i = 0; i < MAX_SCALES; i++) {
    setupScale(i, doutPins_[i], clkPins_[i]);
  }
}

void FilamentScale::tareCurrentScale() {
  if (!waitForScaleReady(currentScale_, logWarn, "not ready to tare within timeout.")) {
    return;
  }

  scale_[currentScale_].tare();
  char message[32];
  snprintf(message, sizeof(message), "Tared scale %d", currentScale_);
  logInfo(message);
}

bool FilamentScale::readCurrentWeightGrams(float* outWeightG) {
  if (!scale_[currentScale_].is_ready()) {
    return false;
  }
  *outWeightG = scale_[currentScale_].get_units(5) * 1000.0f;
  return true;
}

bool FilamentScale::setScaleWeightGrams(int index, float desiredWeightG) {
  if (index < 0 || index >= MAX_SCALES) {
    return false;
  }
  if (desiredWeightG < MIN_KNOWN_WEIGHT_G) {
    return false;
  }
  if (desiredWeightG > MAX_KNOWN_WEIGHT_G) {
    return false;
  }
  if (!waitForScaleReady(index, logWarn, "not ready to set weight within timeout.")) {
    return false;
  }

  long raw = scale_[index].read_average(5);
  float scaleFactor = scale_[index].get_scale();
  float desiredWeightKg = desiredWeightG / 1000.0f;
  long newOffset = raw - static_cast<long>(desiredWeightKg * scaleFactor);
  scale_[index].set_offset(newOffset);
  int weightRounded = static_cast<int>(lroundf(desiredWeightG));
  char message[64];
  snprintf(message, sizeof(message), "Set scale %d to %d g", index, weightRounded);
  logInfo(message);
  return true;
}
