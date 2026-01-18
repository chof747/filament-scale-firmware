# Feature Specification: UI interaction

**Feature Branch**: `[002-ui-interaction]`  
**Created**: 2026-01-17  
**Status**: Draft  
**Input**: User description: "ui interaction -- specify the UI interaction feature by retrofitting the feature to the existing code regarding: displaying the weights via the SSD1306 display and the handling of the button in the main.cpp."

## User Scenarios & Testing *(mandatory)*

### User Story 1 - View current scale on display (Priority: P1)

As a user, I need to see the current scale number and its weight on the OLED so
I can monitor filament usage at a glance.

**Why this priority**: The display is the primary local feedback for the device.

**Independent Test**: Can be tested by observing the OLED while weight changes
and confirming the scale label and weight update.

**Acceptance Scenarios**:

1. **Given** the device is running, **When** a weight change exceeds the display
   step, **Then** the OLED shows the current scale label and weight in grams.
2. **Given** the OLED is initialized, **When** the device starts, **Then** it
   shows a ready message before live weights appear.

---

### User Story 2 - Switch active scale (Priority: P2)

As a user, I need to switch the active scale with a short button press so I can
view each scale’s weight.

**Why this priority**: Switching scales is required to see all three readings.

**Independent Test**: Can be tested by pressing the button and verifying the
scale label changes on the OLED.

**Acceptance Scenarios**:

1. **Given** the button is pressed and released quickly, **When** the press
   duration is under 1 second, **Then** the active scale advances by one.
2. **Given** the active scale advances, **When** the next display update occurs,
   **Then** the OLED shows the new scale label.

---

### User Story 3 - Tare via long press (Priority: P3)

As a user, I need to tare the current scale by holding the button so I can
reset the weight without external tools.

**Why this priority**: Tare is a common, quick action that should be available
without a host.

**Independent Test**: Can be tested by holding the button and verifying the
weight resets near 0 g.

**Acceptance Scenarios**:

1. **Given** the button is held down, **When** the press duration reaches 1
   second, **Then** the current scale is tared once.
2. **Given** a long press is in progress, **When** the button remains held,
   **Then** the tare is not repeated.

---

### Edge Cases

- OLED initialization fails at startup.
- Button bounce causing rapid state changes.
- Press duration near the 1 second threshold.
- Display updates while weight is unchanged.

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: System MUST initialize the SSD1306 display on startup and show a
  "Scale ready" message.
- **FR-002**: System MUST display the current scale label and weight in grams.
  The label uses zero-based numbering (S0, S1, S2).
- **FR-003**: System MUST update the display only when weight changes by at
  least 1 g.
- **FR-004**: System MUST read the MOD button using a pull-up input and debounce
  changes with a greater-than 30 ms threshold.
- **FR-005**: System MUST treat a press shorter than 1 second as a scale
  advance.
- **FR-006**: System MUST treat a press of 1 second or longer as a tare
  operation.
- **FR-007**: System MUST not repeat the tare action while the button remains
  held.
- **FR-008**: System MUST update the display on a fixed interval of 200 ms.

### Key Entities *(include if feature involves data)*

- **Display State**: Current scale label and last displayed weight.
- **Button State**: Last button state, last change timestamp, and press start.

## Assumptions & Dependencies

- The MOD button is wired to `MODE_BUTTON_PIN` with a pull-up.
- The OLED uses the configured address and dimensions in config.
- Display updates can be skipped when weight changes are below 1 g.

## Clarifications

### Session 2026-01-17

- Q: Should the display refresh immediately when the scale changes, even if the
  weight delta is below 1 g? → A: Yes, refresh on scale change.
- Q: Should the display show a weight within 1 second of startup even if the
  weight is unchanged? → A: Yes, render a weight after startup regardless of
  delta.
- Q: Should debounce accept exactly 30 ms or only greater than 30 ms? → A: Use
  greater than 30 ms.
- Q: Should scale labels be zero-based or one-based? → A: Zero-based (S0-S2).

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: The OLED shows a scale label and weight within 1 second of startup.
- **SC-002**: Short press advances the scale on the next release.
- **SC-003**: Long press taring occurs once per press and completes within 1
  second.
- **SC-004**: Display updates occur at most every 200 ms and only when weight
  changes by at least 1 g.
