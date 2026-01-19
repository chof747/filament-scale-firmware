#ifndef LOGGING_H
#define LOGGING_H

#include <Arduino.h>

void logInfo(const char* msg);
void logWarn(const char* msg);
void logError(const char* msg);
void metricFloat(const char* measurement, const char* sensor, float value, const char* unit);
void metricNullableFloat(const char* measurement,
                         const char* sensor,
                         const float* value,
                         const char* unit);

#endif // LOGGING_H
