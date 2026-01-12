
#include "logging.h"
#include "config.h"
#include "commands.h"
#include "scale.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>

static FilamentScale filamentScale(calibrationFactors, doutPins);

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

  String scaleText = "S" + String(filamentScale.getCurrentScale());
  int16_t sx1, sy1;
  uint16_t sw, sh;

  display.getTextBounds(scaleText, 0, 0, &sx1, &sy1, &sw, &sh);
  display.fillRect(0, START_Y - 2, sw + 2, START_Y + sh - 6, SSD1306_WHITE);
  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  display.setCursor(1, START_Y);
  display.print(scaleText);

  display.setTextColor(SSD1306_WHITE);

  String weightText = String(weight, 0) + " g";
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


// Update weight display and emit metric if weight changed sufficiently
// ============================================================================================
void updateWeightDisplayAndMetric() {
  float weightG = 0.0f;
  if (!filamentScale.readCurrentWeightGrams(&weightG)) {
    return;
  }
  static float lastReportedWeightG = 0.0f;
  static bool hasLastWeight = false;
  if (!hasLastWeight || fabsf(weightG - lastReportedWeightG) >= WEIGHT_STEP_G) {
    lastReportedWeightG = weightG;
    hasLastWeight = true;
    metricFloat("weight", ("s_" + String(filamentScale.getCurrentScale())).c_str(), weightG, "g");
    displayWeight(weightG);
  }
}

// Setup function
//=============================================================================================
void setup() {
  Serial.begin(9600);
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  setCommandScale(&filamentScale);

  filamentScale.setupScales();

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
        logInfo(("Switched to scale " + String(filamentScale.getCurrentScale())).c_str());
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
    updateWeightDisplayAndMetric();
  }
}
//=============================================================================================
