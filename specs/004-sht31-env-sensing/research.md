# Research: Environmental Sensing Display & Metrics

## Decision 1: Use cached periodic readings for display

- **Decision**: Use the most recent cached temperature/humidity readings from the periodic sampling loop for display screens.
- **Rationale**: Keeps UI responsive and avoids blocking reads on button presses.
- **Alternatives considered**: Trigger a fresh read on screen entry; show only last logged values.

## Decision 2: Logging interval

- **Decision**: Log metrics every 60 seconds.
- **Rationale**: Balances monitoring usefulness and log volume.
- **Alternatives considered**: 10 seconds, 30 seconds, 5 minutes.

## Decision 3: Unavailable reading handling

- **Decision**: Always show temperature/humidity screens; display an explicit unavailable indicator if the reading is invalid.
- **Rationale**: Maintains consistent cycle order and prevents misinterpretation of missing data.
- **Alternatives considered**: Skip screens when unavailable; show last known values.

## Decision 4: Display labeling

- **Decision**: Use labeled values with units (e.g., "T: 23.4C", "H: 45%") and an unavailable token when needed.
- **Rationale**: Improves clarity on a small display and matches operator expectations.
- **Alternatives considered**: Units-only display; unlabeled values.

## Decision 5: Metric log format alignment

- **Decision**: Emit metrics and log messages using the JSON line format defined in the serial protocol feature (type, ts_ms, sensor, measurement, value, unit; and type, ts_ms, lvl, msg for logs).
- **Rationale**: Keeps telemetry parsing consistent across features.
- **Alternatives considered**: Custom schema with validity fields; separate environmental payload shape.
