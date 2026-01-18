# Feature Specification: Serial protocol

**Feature Branch**: `[003-serial-protocol]`  
**Created**: 2026-01-17  
**Status**: Draft  
**Input**: User description: "serial protocol -- specify the serial output feature by retrofitting the feature to the existing code (commands/metric output)"

## User Scenarios & Testing *(mandatory)*

### User Story 1 - Receive weight metrics (Priority: P1)

As a host system, I need to receive structured weight metrics over serial so I
can track filament usage reliably.

**Why this priority**: Metrics are the primary data output to the host.

**Independent Test**: Can be tested by watching serial output while weight
changes and confirming metric messages include scale, measurement, and grams.

**Acceptance Scenarios**:

1. **Given** the device is running, **When** a weight update is emitted, **Then**
   the host receives a metric line containing sensor, measurement, value, unit,
   and timestamp fields.
2. **Given** the active scale changes, **When** the next weight update is
   emitted, **Then** the sensor identifier reflects the new scale.

---

### User Story 2 - Set known weight via command (Priority: P2)

As a host system, I need to set a known weight via serial command so I can
correct the scale without physical UI interaction.

**Why this priority**: Host control enables calibration workflows.

**Independent Test**: Can be tested by sending a set command and confirming the
next read matches the desired weight.

**Acceptance Scenarios**:

1. **Given** a valid command with a weight in grams, **When** it is received,
   **Then** the device attempts to set the scale weight for the specified or
   current scale.
2. **Given** an invalid scale index or missing weight, **When** a command is
   received, **Then** the device emits a warning message.

---

### User Story 3 - Receive log messages (Priority: P3)

As a host system, I need structured log messages over serial so I can
understand warnings and errors.

**Why this priority**: Logs provide diagnostics during setup and use.

**Independent Test**: Can be tested by triggering a warning condition and
confirming the log message is emitted with level and timestamp.

**Acceptance Scenarios**:

1. **Given** a warning or error occurs, **When** it is logged, **Then** the host
   receives a structured log line with level, message, and timestamp.

---

### Edge Cases

- Serial input exceeds the maximum command length.
- Commands sent without a newline terminator.
- Invalid command names.
- Weight set commands with non-numeric values.

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: System MUST emit metric messages as JSON lines with fields: type,
  ts_ms, sensor, measurement, value, and optional unit.
- **FR-002**: Metric messages MUST use `type` = "metric" and include
  `measurement = weight` and `unit = g` (units align with
  `specs/001-scale-sensing/spec.md`).
- **FR-003**: Sensor identifiers MUST be formatted as `s_<scaleIndex>` using
  zero-based indices (labeling aligns with `specs/002-ui-interaction/spec.md`).
- **FR-004**: System MUST emit log messages as JSON lines with fields: type,
  ts_ms, lvl, and msg.
- **FR-005**: Log messages MUST use `type` = "log" and include a severity level
  of INFO, WARN, or ERROR.
- **FR-006**: System MUST accept serial commands terminated by newline and
  ignore carriage returns.
- **FR-007**: System MUST accept command names `set`, `weight`, and
  `set_weight`.
- **FR-008**: System MUST allow commands in the form `set <weight_g>` to apply
  to the current scale.
- **FR-009**: System MUST allow commands in the form `set <scaleIndex>
  <weight_g>` to apply to a specified scale.
- **FR-010**: System MUST warn on missing arguments with a usage message.
- **FR-011**: System MUST warn on invalid scale indices.
- **FR-012**: System MUST warn when set-known-weight fails.
- **FR-016**: System MUST warn with \"Scale not initialized.\" when set commands
  are received before scale initialization.
- **FR-013**: System MUST treat negative weights as invalid for the set command.
- **FR-015**: System MUST follow the set-known-weight bounds defined in
  `specs/001-scale-sensing/spec.md` and fail the set operation if out of range.
- **FR-014**: Serial command buffer length MUST be limited to 64 characters.

### Key Entities *(include if feature involves data)*

- **Metric Message**: JSON line containing type, ts_ms, sensor, measurement,
  value, and unit.
- **Log Message**: JSON line containing type, ts_ms, lvl, and msg.
- **Command**: Single-line text command with name and optional arguments.

## Assumptions & Dependencies

- Serial communication uses a single line per message and newline terminators.
- Metrics are emitted when the firmware decides to publish a weight update.
- Commands are processed only when the scale system is initialized.
- Shared constraints for units, bounds, and scale indexing are defined in
  `specs/001-scale-sensing/spec.md` and `specs/002-ui-interaction/spec.md`.

## Clarifications

### Session 2026-01-17

- Q: Should JSON field names use timestamp/level/message or ts_ms/lvl/msg? → A:
  Use ts_ms, lvl, msg.
- Q: Should metric JSON output be valid JSON? → A: Yes, fix metric JSON output
  to be valid.
- Q: Should the set command enforce the 0-2500 g bound via the scale API failure?
  → A: Yes, document the limit and failure warning.
- Q: Should commands before initialization warn \"Scale not initialized.\"? → A:
  Yes, document the warning.

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: Host receives well-formed metric JSON lines for weight updates.
- **SC-002**: Invalid commands or arguments produce a warning log line.
- **SC-003**: Set command updates the scale when valid and produces a warning
  when invalid.
