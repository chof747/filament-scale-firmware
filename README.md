# filament-scale-firmware

Firmware for a multi-scale filament weighing setup with an OLED UI and optional environmental sensing.

## Features

- Up to 3 HX711-based scales with per-scale calibration and tare
- OLED display showing current scale and weight
- Mode button to cycle scales and view environment readings
- Optional SHT31 temperature and humidity sampling
- JSON-formatted serial logs and metrics
- Serial command for setting known weight (calibration)

## API

Serial protocol details are documented in `docs/api.md`.
