#include "logging.h"

namespace {
void printEscaped(const char* s) {
  for (const char* p = s; *p; ++p) {
    if (*p == '\\' || *p == '\"') {
      Serial.write('\\');
    }
    Serial.write(*p);
  }
}

void emitJson(const char* type,
              const char* level,
              const char* msg) {
  Serial.print(F("{\"type\":\""));
  Serial.print(type);
  Serial.print(F("\",\"ts_ms\":"));
  Serial.print(millis());
  if (level) {
    Serial.print(F(",\"lvl\":\""));
    Serial.print(level);
    Serial.print('\"');
  }
  if (msg) {
    Serial.print(F(",\"msg\":\""));
    printEscaped(msg);
    Serial.print('\"');
  }
  Serial.println('}');
}
}  // namespace

void logInfo(const char* msg) {
  emitJson("log", "INFO", msg);
}

void logWarn(const char* msg) {
  emitJson("log", "WARN", msg);
}

void logError(const char* msg) {
  emitJson("log", "ERROR", msg);
}

void metricFloat(const char* measurement, const char* sensor, float value, const char* unit) {
  Serial.print(F("{\"type\":\"metric\",\"ts_ms\":"));
  Serial.print(millis());
  Serial.print(F(",\"sensor\":\""));
  Serial.print(sensor);
  Serial.print(F("\",\"measurement\":\""));
  Serial.print(measurement);
  Serial.print(F("\",\"value\":"));
  Serial.print(value, 6);
  if (unit) {
    Serial.print(F(",\"unit\":\""));
    Serial.print(unit);
    Serial.print('\"');
  }
  Serial.println('}');
}

void metricNullableFloat(const char* measurement,
                         const char* sensor,
                         const float* value,
                         const char* unit) {
  Serial.print(F("{\"type\":\"metric\",\"ts_ms\":"));
  Serial.print(millis());
  Serial.print(F(",\"sensor\":\""));
  Serial.print(sensor);
  Serial.print(F("\",\"measurement\":\""));
  Serial.print(measurement);
  Serial.print(F("\",\"value\":"));
  if (value) {
    Serial.print(*value, 6);
    if (unit) {
      Serial.print(F(",\"unit\":\""));
      Serial.print(unit);
      Serial.print('\"');
    }
  } else {
    Serial.print(F("null"));
  }
  Serial.println('}');
}
