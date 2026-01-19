#include "sht31.h"

#include <math.h>

#include "config.h"

EnvironmentalSensor::EnvironmentalSensor()
    : initialized(false),
      lastSampleMs(0) {
  cachedReading.temperatureC = 0.0f;
  cachedReading.humidityPercent = 0.0f;
  cachedReading.isValid = false;
  cachedReading.timestampMs = 0;
}

bool EnvironmentalSensor::begin() {
  initialized = sensor.begin(ENV_SENSOR_ADDR);
  lastSampleMs = millis() - ENV_SAMPLE_INTERVAL_MS;
  if (!initialized) {
    cachedReading.isValid = false;
    cachedReading.timestampMs = millis();
  }
  return initialized;
}

bool EnvironmentalSensor::sampleIfDue(unsigned long now_ms) {
  if (now_ms - lastSampleMs < ENV_SAMPLE_INTERVAL_MS) {
    return false;
  }
  lastSampleMs = now_ms;
  readSensor(&cachedReading, now_ms);
  return true;
}

const EnvReading& EnvironmentalSensor::lastReading() const {
  return cachedReading;
}

bool EnvironmentalSensor::readSensor(EnvReading* out, unsigned long now_ms) {
  out->timestampMs = now_ms;
  if (!initialized) {
    out->isValid = false;
    return false;
  }

  float temp_c = sensor.readTemperature();
  float humidity = sensor.readHumidity();

  if (isnan(temp_c) || isnan(humidity)) {
    out->isValid = false;
    return false;
  }

  out->temperatureC = temp_c;
  out->humidityPercent = humidity;
  out->isValid = isReadingInRange(*out);
  return out->isValid;
}

bool EnvironmentalSensor::isReadingInRange(const EnvReading& reading) const {
  if (reading.temperatureC < ENV_TEMP_MIN_C || reading.temperatureC > ENV_TEMP_MAX_C) {
    return false;
  }
  if (reading.humidityPercent < ENV_HUM_MIN_PCT || reading.humidityPercent > ENV_HUM_MAX_PCT) {
    return false;
  }
  return true;
}
