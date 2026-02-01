
#include "logging.h"
#include "config.h"
#include "commands.h"
#include "scale.h"
#include "sht31.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>
#include <stdio.h>

static FilamentScale filamentScale(calibrationFactors, doutPins, clkPins);
static EnvironmentalSensor envSensor;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
unsigned long lastButtonChangeMs = 0;
unsigned long buttonPressStartMs = 0;
bool lastButtonState = HIGH;
bool longPressTriggered = false;


// Setup OLED display
// =============================================================================================
void setupDisplay() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    logError("SSD1306 allocation failed.");
    return;
  }

  display.clearDisplay();
  display.setRotation(2);
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Scale ready");
  display.display();
}

// Display weight on OLED
// =============================================================================================
void displayWeight(float weight) {
  display.clearDisplay();
  display.setTextSize(2);

  char scaleText[6];
  snprintf(scaleText, sizeof(scaleText), "S%d", filamentScale.getCurrentScale());
  int16_t sx1, sy1;
  uint16_t sw, sh;

  display.getTextBounds(scaleText, 0, 0, &sx1, &sy1, &sw, &sh);
  display.fillRect(0, START_Y - 2, sw + 2, START_Y + sh - 6, SSD1306_WHITE);
  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  display.setCursor(1, START_Y);
  display.print(scaleText);

  display.setTextColor(SSD1306_WHITE);

  int weightRounded = static_cast<int>(lroundf(weight));
  char weightText[12];
  snprintf(weightText, sizeof(weightText), "%d g", weightRounded);
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(weightText, 0, 0, &x1, &y1, &w, &h);
  int16_t x = 0;
  if (w < SCREEN_WIDTH) {
    x = SCREEN_WIDTH - w;
  }
  display.setCursor(x, START_Y);
  display.print(weightText);
  display.display();
}

void displayEnvReadingCommon(const char* text) {
  display.clearDisplay();
  display.setTextSize(2);
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  int16_t x = 0;
  if (w < SCREEN_WIDTH) {
    x = SCREEN_WIDTH - w;
  }
  display.setCursor(x, START_Y);
  display.print(text);
  display.display();
}

// Display environmental reading (temperature)
// =============================================================================================
void displayEnvTemperature(const EnvReading& reading) {
  char text[24];
  filamentScale.formatTemperatureLabel(reading, text, sizeof(text));
  displayEnvReadingCommon(text);
}

// Display environmental reading (humidity)
// =============================================================================================
void displayEnvHumidity(const EnvReading& reading) {
  char text[24];
  filamentScale.formatHumidityLabel(reading, text, sizeof(text));
  displayEnvReadingCommon(text);
}

// Update weight display and emit metric if weight changed sufficiently
// ============================================================================================
void updateWeightDisplayAndMetric() {
  float weightG = 0.0f;
  if (!filamentScale.readCurrentWeightGrams(&weightG)) {
    return;
  }
  static float lastReportedWeightG = 0.0f;
  static bool hasLastWeight = false;
  static int lastScale = -1;
  static bool forceDisplay = true;
  int currentScale = filamentScale.getCurrentScale();
  if (currentScale != lastScale) {
    lastScale = currentScale;
    forceDisplay = true;
  }
  if (forceDisplay || !hasLastWeight || fabsf(weightG - lastReportedWeightG) >= WEIGHT_STEP_G) {
    lastReportedWeightG = weightG;
    hasLastWeight = true;
    forceDisplay = false;
    char sensorLabel[8];
    snprintf(sensorLabel, sizeof(sensorLabel), "s_%d", filamentScale.getCurrentScale());
    metricFloat("weight", sensorLabel, weightG, "g");
    displayWeight(weightG);
  }
}

// Emit periodic environmental metrics and log failures
// ============================================================================================
void updateEnvMetrics(unsigned long now) {
  if (!envSensor.sampleIfDue(now)) {
    return;
  }
  const EnvReading& reading = envSensor.lastReading();
  if (reading.isValid) {
    metricNullableFloat("temperature", "env", &reading.temperatureC, "C");
    metricNullableFloat("humidity", "env", &reading.humidityPercent, "%");
  } else {
    metricNullableFloat("temperature", "env", NULL, NULL);
    metricNullableFloat("humidity", "env", NULL, NULL);
    logWarn("Env sensor read failed.");
  }
}

// Setup function
//=============================================================================================
void setup() {
  Serial.begin(9600);
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  setCommandScale(&filamentScale);

  filamentScale.setupScales();
  if (!envSensor.begin()) {
    logWarn("Env sensor not detected.");
  }

  setupDisplay();
}

// Main loop
//=============================================================================================
void loop() {
  unsigned long now = millis();
  pollSerialCommands();
  bool buttonState = digitalRead(MODE_BUTTON_PIN);
  if (buttonState != lastButtonState && (now - lastButtonChangeMs) > 30) {
    lastButtonChangeMs = now;
    lastButtonState = buttonState;

    if (buttonState == LOW) {
      buttonPressStartMs = now;
      longPressTriggered = false;
    } else {
      unsigned long pressDuration = now - buttonPressStartMs;
      if (!longPressTriggered && pressDuration < 1000) {
        filamentScale.advanceScale();
        char message[32];
        snprintf(message, sizeof(message), "Switched to scale %d", filamentScale.getCurrentScale());
        logInfo(message);
      }
    }
  }

  if (buttonState == LOW && !longPressTriggered) {
    if (now - buttonPressStartMs >= 1000) {
      filamentScale.tareCurrentScale();
      longPressTriggered = true;
    }
  }

  static unsigned long lastDisplayMs = 0;
  if (now - lastDisplayMs >= DISPLAY_INTERVAL_MS) {
    lastDisplayMs = now;
    if (filamentScale.isDisplayingScale()) {
      updateWeightDisplayAndMetric();
    } else if (filamentScale.isDisplayingEnvTemp()) {
      displayEnvTemperature(envSensor.lastReading());
    } else if (filamentScale.isDisplayingEnvHum()) {
      displayEnvHumidity(envSensor.lastReading());
    }
  }

  updateEnvMetrics(now);
}
//=============================================================================================
