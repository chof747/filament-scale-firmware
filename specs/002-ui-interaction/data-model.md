# Data Model: UI interaction

## Entities

### Display State

- **scale_label**: current scale label (S0, S1, S2)
- **last_weight_g**: last displayed weight value
- **has_last_weight**: whether a weight has been rendered

### Button State

- **last_button_state**: previous digital state
- **last_change_ms**: timestamp of last state change
- **press_start_ms**: timestamp when button was pressed
- **long_press_triggered**: whether tare was triggered for the current press

## Validation Rules

- Debounce only accepts changes after >30 ms.
- Long press triggers at >=1 second and only once per press.

## State Transitions

- **Display State**: updates on weight delta >= 1 g, on scale change, or after
  startup when first weight is rendered.
- **Button State**: idle -> pressed on LOW transition; pressed -> released on
  HIGH transition; long_press_triggered set once per long press.
