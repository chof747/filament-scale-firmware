#include "scale.h"

#include <Arduino.h>

#include "logging.h"

namespace {
const float kDefaultCalibrationFactors[MAX_SCALES] = {411450.0f, 100000.0f, 10000.0f};
const int kDefaultDoutPins[MAX_SCALES] = {DOUT1, DOUT2, DOUT3};
}

FilamentScale::FilamentScale()
    : FilamentScale(kDefaultCalibrationFactors, kDefaultDoutPins) {}

FilamentScale::FilamentScale(const float* calibrationFactors, const int* doutPins)
    : currentScale_(0) {
  for (int i = 0; i < MAX_SCALES; i++) {
    calibration_factor_[i] = calibrationFactors[i];
    doutPins_[i] = doutPins[i];
  }
}

bool FilamentScale::waitForScaleReady(int index, void (*logFn)(const char*), const char* timeoutMsg) {
  unsigned long startMillis = millis();
  while (!scale_[index].is_ready()) {
    if (millis() - startMillis > TIMEOUT_MS) {
      logFn((String("Scale ") + String(index) + " " + timeoutMsg).c_str());
      return false;
    }
    delay(10);
  }
  return true;
}

void FilamentScale::setupScale(int index, int doutPin) {
  scale_[index].begin(doutPin, CLK);

  if (!waitForScaleReady(index, logError, "not ready within timeout. Check wiring!")) {
    return;
  }

  scale_[index].set_scale(calibration_factor_[index]);
  scale_[index].tare(); //Reset the scale to 0
  logInfo(("Scale " + String(index) + " setup complete.").c_str());
}

int FilamentScale::getCurrentScale() const {
  return currentScale_;
}

void FilamentScale::setCurrentScale(int index) {
  if (index >= 0 && index < MAX_SCALES) {
    currentScale_ = index;
  }
}

void FilamentScale::advanceScale() {
  currentScale_ = (currentScale_ + 1) % MAX_SCALES;
}

void FilamentScale::setupScales() {
  for (int i = 0; i < MAX_SCALES; i++) {
    setupScale(i, doutPins_[i]);
  }
}

void FilamentScale::tareCurrentScale() {
  if (!waitForScaleReady(currentScale_, logWarn, "not ready to tare within timeout.")) {
    return;
  }

  scale_[currentScale_].tare();
  logInfo(("Tared scale " + String(currentScale_)).c_str());
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
  if (desiredWeightG < 0.0f) {
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
  logInfo(("Set scale " + String(index) + " to " + String(desiredWeightG, 0) + " g").c_str());
  return true;
}
