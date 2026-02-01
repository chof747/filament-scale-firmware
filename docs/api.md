# Filament Scale Serial API

This document describes the JSON messages the firmware emits over serial and the serial commands it accepts.

## Serial Transport

- Baud rate: `9600`
- Line endings: each JSON message is a single line terminated by `\n`.
- Command input is line-based; commands are parsed when a `\n` is received.
- Max command length: `SERIAL_CMD_MAX_LEN` (currently 64 chars).

## Outbound JSON Messages

All outbound messages are JSON objects on a single line.

### Log Message

Emitted by `logInfo`, `logWarn`, `logError`.

```json
{
  "type": "log",
  "ts_ms": 123456,
  "lvl": "INFO",
  "msg": "Scale 0 setup complete."
}
```

Fields:
- `type` (string): `"log"`
- `ts_ms` (number): milliseconds since boot (`millis()`).
- `lvl` (string): `"INFO"`, `"WARN"`, or `"ERROR"`.
- `msg` (string): message text, with `"` and `\` escaped.

Common log messages include:
- Scale setup: `"Scale <n> setup complete."`
- Scale not ready timeouts: `"Scale <n> <reason>"`
- Tare: `"Tared scale <n>"`
- Set weight: `"Set scale <n> to <g> g"`
- Env sensor failures: `"Env sensor not detected."`, `"Env sensor read failed."`
- UI events: `"Switched to scale <n>"`

### Metric Message (float)

Emitted by `metricFloat`.

```json
{
  "type": "metric",
  "ts_ms": 123456,
  "sensor": "s_0",
  "measurement": "weight",
  "value": 742.125000,
  "unit": "g"
}
```

Fields:
- `type` (string): `"metric"`
- `ts_ms` (number): milliseconds since boot (`millis()`).
- `sensor` (string): source label.
- `measurement` (string): metric name.
- `value` (number): floating-point value with 6 decimals.
- `unit` (string, optional): unit label.

Weight metrics:
- `sensor`: `"s_0"`, `"s_1"`, `"s_2"` (current scale index)
- `measurement`: `"weight"`
- `unit`: `"g"`

### Metric Message (nullable float)

Emitted by `metricNullableFloat`. If the sensor reading is invalid, `value` is `null`
and `unit` is omitted.

```json
{
  "type": "metric",
  "ts_ms": 123456,
  "sensor": "env",
  "measurement": "temperature",
  "value": null
}
```

Environmental metrics:
- Temperature:
  - `sensor`: `"env"`
  - `measurement`: `"temperature"`
  - `unit`: `"C"` when valid
- Humidity:
  - `sensor`: `"env"`
  - `measurement`: `"humidity"`
  - `unit`: `"%"` when valid

## Inbound Serial Commands

Commands are ASCII text lines (terminated by `\n`).
Arguments are separated by spaces or tabs.

### Set Scale Weight (Calibration)

Aliases: `set`, `weight`, `set_weight`

Usage:
```
set <weight_g>
set <scaleIndex> <weight_g>
```

Behavior:
- If `<scaleIndex>` is omitted, the current scale is used.
- `scaleIndex` must be `0..(MAX_SCALES-1)`.
- `weight_g` must be a float >= `0.0`.
- On success, the scale offset is adjusted to match the provided known weight.

Examples:
```
set 750
set 1 1000
weight 2 500
```

Errors (returned as log messages):
- `Scale not initialized.`
- `Usage: set <scaleIndex?> <weight_g>`
- `Invalid scale index.`
- `Weight must be >= 0.`
- `Failed to set weight.`
