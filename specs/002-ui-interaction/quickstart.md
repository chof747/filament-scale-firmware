# Quickstart: UI interaction

## Goal

Confirm OLED output and button behaviors on the device.

## Steps

1. Build and upload the firmware with PlatformIO.
2. Verify the OLED shows "Scale ready" at startup.
3. Wait for the first weight render and confirm a scale label with grams.
4. Short press the button and confirm the scale label advances.
5. Long press the button (>=1 second) and confirm the current scale tars once.

## Expected Results

- OLED shows the scale label and weight.
- Short press advances scale; long press tares once.
- Display refreshes after scale change and within 1 second of startup.
