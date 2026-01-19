# Quickstart: Environmental Sensing Display & Metrics

## Goal

Validate that temperature/humidity readings are sampled every 60 seconds, logged as metrics, and shown in the display cycle after the last scale.

## Prerequisites

- Device wired with the SHT31-D sensor on I2C
- PlatformIO configured for the Arduino Nano target

## Steps

1. Build and upload firmware with PlatformIO.
2. Open the serial monitor and confirm periodic metrics logs appear every 60 seconds as JSON lines with fields: type, ts_ms, sensor, measurement, value, optional unit.
3. Use the mode button to cycle through scales until the last scale is shown.
4. Press mode again to show temperature ("T: ..."); press again to show humidity ("H: ...").
5. Press mode once more and confirm the display returns to scale 0.
6. Simulate a sensor read failure (disconnect or block the sensor) and confirm the display shows the unavailable indicator, a metric line emits `value = null`, and a WARN log line is emitted.

## Expected Results

- Metrics logs contain timestamped temperature and humidity values every 60 seconds.
- Display shows temperature then humidity after the last scale, returning to scale 0 on the next press.
- Unavailable readings are explicitly indicated on screen and in logs.
