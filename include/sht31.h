#ifndef SHT31_H
#define SHT31_H

#include <Arduino.h>

#include <Adafruit_SHT31.h>

struct EnvReading {
  float temperatureC;
  float humidityPercent;
  bool isValid;
  unsigned long timestampMs;
};

class EnvironmentalSensor {
 public:
  EnvironmentalSensor();

  bool begin();
  bool sampleIfDue(unsigned long now_ms);
  const EnvReading& lastReading() const;

 private:
  bool readSensor(EnvReading* out, unsigned long now_ms);
  bool isReadingInRange(const EnvReading& reading) const;

  Adafruit_SHT31 sensor;
  bool initialized;
  unsigned long lastSampleMs;
  EnvReading cachedReading;
};

#endif  // SHT31_H
