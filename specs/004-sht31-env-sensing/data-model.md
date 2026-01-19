# Data Model: Environmental Sensing Display & Metrics

## Entity: Environmental Reading

**Description**: Point-in-time temperature and humidity values with validity and timestamp.

**Fields**:
- **timestamp**: time the reading was captured
- **temperature_c**: numeric temperature in degrees Celsius
- **humidity_percent**: numeric humidity in percent relative humidity
- **is_valid**: boolean indicating whether the reading is valid

**Relationships**:
- None (single, independent readings)

**Validation Rules**:
- If `is_valid` is false, temperature and humidity are treated as unavailable.
- Display uses the most recent cached reading (valid or unavailable).

**State Transitions**:
- `valid` -> `invalid` when read fails
- `invalid` -> `valid` when a subsequent read succeeds
