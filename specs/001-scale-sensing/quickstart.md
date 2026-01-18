# Quickstart: Scale sensing

## Goal

Confirm scale sensing reads and tare behavior on the device.

## Steps

1. Build and upload the firmware with PlatformIO.
2. Power the device with three connected scales.
3. Observe weight readings on the display or serial output.
4. Trigger tare for the current scale and confirm the next reading is near 0 g.

## Expected Results

- Each scale reports a stable, non-negative weight.
- Tare resets the current scale to near 0 g.
- If a scale is not ready, operations return quickly without blocking.
