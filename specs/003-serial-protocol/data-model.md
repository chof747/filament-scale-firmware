# Data Model: Serial protocol

## Entities

### Metric Message

- **type**: "metric"
- **ts_ms**: timestamp in milliseconds
- **sensor**: `s_<scaleIndex>`
- **measurement**: "weight"
- **value**: numeric weight value
- **unit**: "g"

### Log Message

- **type**: "log"
- **ts_ms**: timestamp in milliseconds
- **lvl**: INFO, WARN, or ERROR
- **msg**: log message string

### Command

- **name**: "set", "weight", or "set_weight"
- **args**: optional scale index and required weight

## Validation Rules

- Commands must be newline-terminated and fit within 64 characters.
- Negative weights are rejected; out-of-range weights fail in scale logic.

## State Transitions

- **Command processing**: idle -> parse -> execute -> log warnings on failure.
