# Quickstart: Serial protocol

## Goal

Verify serial output for metrics and logs, and set commands.

## Steps

1. Build and upload the firmware with PlatformIO.
2. Open the serial monitor at 9600 baud.
3. Observe metric output while weight changes.
4. Send `set <weight_g>` and confirm the weight adjusts.
5. Send invalid commands and confirm warning logs.

## Expected Results

- Metric lines are well-formed JSON.
- Log lines include level and message.
- Invalid commands produce warning logs.
