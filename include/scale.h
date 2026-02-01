#ifndef SCALE_H
#define SCALE_H

#include <stddef.h>

#include "HX711.h"
#include "config.h"
#include "sht31.h"

class FilamentScale {
 public:
  enum DisplayMode {
    kDisplayScale,
    kDisplayEnvTemp,
    kDisplayEnvHum
  };

  FilamentScale();
  FilamentScale(const float* calibrationFactors, const int* doutPins, const int* clkPins);

  int getCurrentScale() const;
  void setCurrentScale(int index);
  void advanceScale();

  DisplayMode getDisplayMode() const;
  bool isDisplayingScale() const;
  bool isDisplayingEnvTemp() const;
  bool isDisplayingEnvHum() const;
  void formatTemperatureLabel(const EnvReading& reading, char* out, size_t size) const;
  void formatHumidityLabel(const EnvReading& reading, char* out, size_t size) const;

  void setupScales();
  void tareCurrentScale();

  bool readCurrentWeightGrams(float* outWeightG);
  bool setScaleWeightGrams(int index, float desiredWeightG);

 private:
  bool waitForScaleReady(int index, void (*logFn)(const char*), const char* timeoutMsg);
  void setupScale(int index, int doutPin, int clkPin);

  float calibration_factor_[MAX_SCALES];
  int doutPins_[MAX_SCALES];
  int clkPins_[MAX_SCALES];
  HX711 scale_[MAX_SCALES];
  int currentScale_;
  DisplayMode displayMode;
};

#endif // SCALE_H
