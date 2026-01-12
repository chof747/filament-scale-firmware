#ifndef SCALE_H
#define SCALE_H

#include "HX711.h"
#include "config.h"

class FilamentScale {
 public:
  FilamentScale();
  FilamentScale(const float* calibrationFactors, const int* doutPins);

  int getCurrentScale() const;
  void setCurrentScale(int index);
  void advanceScale();

  void setupScales();
  void tareCurrentScale();

  bool readCurrentWeightGrams(float* outWeightG);
  bool setScaleWeightGrams(int index, float desiredWeightG);

 private:
  bool waitForScaleReady(int index, void (*logFn)(const char*), const char* timeoutMsg);
  void setupScale(int index, int doutPin);

  float calibration_factor_[MAX_SCALES];
  int doutPins_[MAX_SCALES];
  HX711 scale_[MAX_SCALES];
  int currentScale_;
};

#endif // SCALE_H
