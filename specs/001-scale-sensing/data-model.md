# Data Model: Scale sensing

## Entities

### Scale

- **scale_index**: 0-2
- **calibration_factor**: fixed constant per scale
- **dout_pin**: configured input pin per scale
- **offset**: computed tare/adjustment value
- **ready_state**: ready or not-ready

### Scale Reading

- **scale_index**: 0-2
- **weight_g**: non-negative weight in grams
- **timestamp_ms**: time of read (optional if tracked by caller)

## Validation Rules

- **weight_g** MUST be non-negative.
- set-known-weight MUST reject values outside 0 g to 2500 g.

## State Transitions

- **ready_state**: not-ready -> ready when HX711 reports ready within timeout.
- **offset**: updated on tare and set-known-weight.
