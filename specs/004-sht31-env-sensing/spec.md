# Feature Specification: Environmental Sensing Display & Metrics

**Feature Branch**: `004-sht31-env-sensing`  
**Created**: 2026-01-18  
**Status**: Draft  
**Input**: User description: "environment sensing -- The firmware needs to access a SHT31-D sensor on I2C and read the temperature and humidity from it in regular intervals. The environmental values need to be reported as metric log messages and when the user cycles through the scales with the mod button after the last scale before returning to scale 0 first the temperature and then the humidity values of the sensors are shown on the display."

## Clarifications

### Session 2026-01-18

- Q: What is the metrics logging interval? → A: 60 seconds.
- Q: When readings are unavailable, should the environment screens still appear? → A: Show temperature/humidity screens with an unavailable indicator.
- Q: What display labeling should be used for temperature and humidity values? → A: Use labeled values (e.g., "T: 23.4°C", "H: 45%") with an unavailable token when needed.
- Q: Which user roles are in scope? → A: Operator and maintainer.
- Q: Which reading should be displayed when the user cycles to the environment screens? → A: Show the most recent cached reading from the periodic interval.

## User Scenarios & Testing *(mandatory)*

**User Roles**: Operator, maintainer.

### User Story 1 - View Environment Readings in Cycle (Priority: P1)

As an operator, I want temperature and humidity to appear after the last scale when I cycle with the mode button so I can quickly check the environment without leaving the device workflow.

**Why this priority**: This is the primary user-visible behavior and enables immediate value from the sensor.

**Independent Test**: Can be fully tested by cycling through scales and confirming temperature then humidity appear before returning to scale 0.

**Acceptance Scenarios**:

1. **Given** the device is showing the last scale, **When** the user presses the mode button, **Then** the display shows the current temperature reading.
2. **Given** the temperature reading is displayed, **When** the user presses the mode button again, **Then** the display shows the current humidity reading.
3. **Given** the humidity reading is displayed, **When** the user presses the mode button again, **Then** the display returns to scale 0.

---

### User Story 2 - Receive Environment Metrics Logs (Priority: P2)

As a maintainer, I want periodic temperature and humidity metrics logged so I can monitor environmental conditions over time.

**Why this priority**: Logs support diagnostics and trend analysis without requiring constant UI interaction.

**Independent Test**: Can be tested by observing log output over a defined interval.

**Acceptance Scenarios**:

1. **Given** the device is running normally, **When** a logging interval elapses, **Then** a metrics log entry is recorded for temperature and humidity.
2. **Given** multiple intervals elapse, **When** logs are reviewed, **Then** each entry includes distinct timestamps and readings.

---

### User Story 3 - Handle Sensor Read Issues Transparently (Priority: P3)

As an operator, I want clear feedback when a reading is unavailable so I do not mistake missing data for real values.

**Why this priority**: Prevents incorrect decisions based on stale or invalid readings.

**Independent Test**: Can be tested by simulating an unavailable sensor and observing display/log behavior.

**Acceptance Scenarios**:

1. **Given** the system cannot read a valid temperature or humidity value, **When** the user reaches the environment display, **Then** the display indicates the reading is unavailable.
2. **Given** a read failure occurs, **When** the next logging interval occurs, **Then** the metrics log records the failure state instead of a numeric value.

---

### Edge Cases

- What happens when the environment sensor is missing or disconnected at startup?
- How does the system handle readings outside expected physical ranges (e.g., negative humidity or extreme temperature)?
- What happens if the user cycles rapidly through displays faster than a new reading is available?

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: System MUST collect temperature and humidity readings every 60 seconds.
- **FR-002**: System MUST emit metric messages as JSON lines with fields: type, ts_ms, sensor, measurement, value, and optional unit, with `type = "metric"`.
- **FR-003**: System MUST show temperature immediately after the last scale when the user cycles forward.
- **FR-004**: System MUST show humidity immediately after the temperature reading when the user cycles forward again.
- **FR-005**: System MUST return to scale 0 after the humidity reading on the next cycle.
- **FR-006**: System MUST present an explicit unavailable indicator when a reading cannot be obtained.
- **FR-008**: Metric messages MUST use `measurement = temperature` or `measurement = humidity` with units "C" and "%" respectively.
- **FR-009**: Metric messages MUST use `sensor = env` to identify the environmental sensor.
- **FR-010**: When a reading is unavailable, the metric message MUST set `value = null` and omit `unit`.
- **FR-011**: System MUST emit a log message as a JSON line with fields: type, ts_ms, lvl, msg when a read failure occurs.
- **FR-012**: Log messages MUST use `type` = "log" and include a severity level of WARN for read failures.
- **FR-013**: System MUST log metrics every 60 seconds.
- **FR-014**: System MUST show the temperature and humidity screens even when readings are unavailable, using the unavailable indicator.
- **FR-015**: Temperature and humidity displays MUST use labeled values (e.g., "T:" and "H:") with units, and show an unavailable token when no reading exists.
- **FR-016**: Displayed temperature and humidity values MUST use the most recent cached readings collected by the periodic interval.

### Key Entities *(include if feature involves data)*

- **Environmental Reading**: A point-in-time temperature and humidity value with timestamp and validity status.

### Assumptions

- Temperature is shown in degrees Celsius and humidity in percent relative humidity.
- Only one environment sensor is present.

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: Temperature appears within 2 seconds after the user cycles past the last scale.
- **SC-002**: Humidity appears within 2 seconds after the user cycles past the temperature display.
- **SC-003**: At least 95% of logging intervals over a 24-hour period produce a metrics entry.
- **SC-004**: Operators can view both temperature and humidity within 10 seconds of starting a cycle from the last scale.
