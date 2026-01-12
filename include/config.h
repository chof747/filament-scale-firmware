#ifndef CONFIG_H
#define CONFIG_H

#define MAX_SCALES 3
#define TIMEOUT_MS 1000

#define CLK 2
#define DOUT1 3
#define DOUT2 4
#define DOUT3 5
#define MODE_BUTTON_PIN A1

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_ADDR 0x3C

#define START_Y 8
#define DISPLAY_INTERVAL_MS 200
#define WEIGHT_STEP_G 1.0f
#define SERIAL_CMD_MAX_LEN 64

extern const float calibrationFactors[MAX_SCALES];
extern const int doutPins[MAX_SCALES];

#endif // CONFIG_H
