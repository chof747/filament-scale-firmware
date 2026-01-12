#include "commands.h"

#include <Arduino.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "logging.h"
#include "scale.h"

static FilamentScale* commandScale = NULL;

void setCommandScale(FilamentScale* scale) {
  commandScale = scale;
}

// Handle a single serial command line
// =============================================================================================
static void handleSerialCommand(char* line) {
  char* cmd = strtok(line, " \t");
  if (!cmd) {
    return;
  }

  if (strcmp(cmd, "set") == 0 || strcmp(cmd, "weight") == 0 || strcmp(cmd, "set_weight") == 0) {
    char* arg1 = strtok(NULL, " \t");
    char* arg2 = strtok(NULL, " \t");

    if (!commandScale) {
      logWarn("Scale not initialized.");
      return;
    }

    int index = commandScale->getCurrentScale();
    const char* weightStr = NULL;
    if (arg1 && arg2) {
      index = atoi(arg1);
      weightStr = arg2;
    } else if (arg1) {
      weightStr = arg1;
    }

    if (!weightStr) {
      logWarn("Usage: set <scaleIndex?> <weight_g>");
      return;
    }
    if (index < 0 || index >= MAX_SCALES) {
      logWarn("Invalid scale index.");
      return;
    }

    float desiredWeightG = atof(weightStr);
    if (desiredWeightG < 0.0f) {
      logWarn("Weight must be >= 0.");
      return;
    }

    if (!commandScale->setScaleWeightGrams(index, desiredWeightG)) {
      logWarn("Failed to set weight.");
    }
  }
}

void pollSerialCommands() {
  static char buffer[SERIAL_CMD_MAX_LEN];
  static size_t len = 0;

  while (Serial.available() > 0) {
    char c = static_cast<char>(Serial.read());
    if (c == '\n') {
      buffer[len] = '\0';
      handleSerialCommand(buffer);
      len = 0;
    } else if (c != '\r') {
      if (len < sizeof(buffer) - 1) {
        buffer[len++] = c;
      }
    }
  }
}
