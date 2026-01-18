# Feature Specification: Scale sensing

**Feature Branch**: `[001-scale-sensing]`  
**Created**: 2026-01-17  
**Status**: Draft  
**Input**: User description: "Scale sensing for 3 HX711 load cells on Arduino Nano"

## User Scenarios & Testing *(mandatory)*

### User Story 1 - Read scale weight (Priority: P1)

As a user of the device, I need weight readings from the currently selected
scale so I can track filament usage accurately.

**Why this priority**: All other features depend on having reliable weight
measurements.

**Independent Test**: Can be tested by reading all three scales and confirming
each returns a non-negative weight in grams.

**Acceptance Scenarios**:

1. **Given** the system is initialized, **When** a read is requested for the
   current scale, **Then** the system returns the current weight in grams.
2. **Given** a scale is not ready, **When** a read is requested, **Then** the
   system returns a "not ready" result within 1 second without logging.

---

### User Story 2 - Tare a scale (Priority: P2)

As a user of the device, I need to tare a selected scale so the current load is
treated as zero for subsequent readings.

**Why this priority**: Tare is needed to handle empty spools and baselines.

**Independent Test**: Can be tested by taring a scale and verifying subsequent
reads are near 0 g under the same load.

**Acceptance Scenarios**:

1. **Given** a scale is ready, **When** it is tared, **Then** the next read
   returns approximately 0 g.
2. **Given** a scale is not ready, **When** a tare is requested, **Then** the
   system returns a "not ready" result within 1 second and logs a warning that
   the scale is not ready to tare within the timeout.

---

### User Story 3 - Set known weight (Priority: P3)

As a user of the device, I need to set a known weight on a scale so the scale
can be corrected to a reference mass without a full recalibration.

**Why this priority**: Enables quick correction when a reference weight is
available.

**Independent Test**: Can be tested by placing a known weight, setting it in
firmware, and verifying the read matches the known weight.

**Acceptance Scenarios**:

1. **Given** a scale is ready and a desired weight between 0 g and 2500 g is
   provided, **When** set-known-weight is invoked, **Then** the next read returns
   the desired weight in grams.
2. **Given** a desired weight outside 0 g to 2500 g, **When** set-known-weight is
   invoked, **Then** the request is rejected with a clear "out of range" result.

---

### Edge Cases

- Scale is disconnected or never becomes ready.
- Rapid repeated read requests without an enforced minimum interval.
- Tare or set-known-weight requested while a scale is not ready.
- Attempt to read a scale index outside the supported range.

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: System MUST support exactly 3 scales.
- **FR-002**: Calibration factors MUST be fixed constants and not user
  configurable at runtime.
- **FR-003**: System MUST initialize each scale with its configured input at
  startup and tare each scale during setup when the scale becomes ready within
  the timeout.
- **FR-004**: System MUST read weight in grams from the current scale on
  request.
- **FR-005**: System MUST return a "not ready" result if a scale is not ready,
  without blocking beyond 1 second.
- **FR-006**: System MUST log a warning when a tare or set-known-weight is
  requested for a not-ready scale, using action-specific messages (e.g., "not
  ready to tare within timeout."). Reads do not log warnings.
- **FR-007**: System MUST allow taring a selected scale.
- **FR-008**: System MUST allow setting a known weight in grams for a specific
  scale within 0 g to 2500 g.
- **FR-009**: System MUST reject negative or out-of-range weights for
  set-known-weight.

### Key Entities *(include if feature involves data)*

- **Scale**: A single load-measurement channel with calibration and readiness
  state.
- **Scale Reading**: A weight value in grams associated with a specific scale.

## Assumptions & Dependencies

- The device always has three connected scales.
- Calibration factors are provided and validated outside of this feature.
- Read requests may occur at any time; the scale class does not rate-limit
  reads.

## Clarifications

### Session 2026-01-17

- Q: Should reads target any scale or only the current scale? → A: Only the
  current scale.
- Q: Should a minimum read interval be enforced in the scale read API? → A: No,
  reads are not rate-limited in the scale class.
- Q: Should not-ready warnings be standardized across read, tare, and set-known
  weight? → A: No, keep action-specific warnings and no warning for reads.
- Q: Should set-known-weight enforce an upper bound? → A: Yes, enforce 0 g to
  2500 g.
- Q: Should setup tare proceed only when the scale is ready? → A: Yes, tare only
  if the scale becomes ready within the timeout.

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: Each scale reports weight within +/- 1 g of a known reference after
  setup.
- **SC-002**: Read and tare requests for a not-ready scale return within 1 second
  and do not block other operations.
- **SC-003**: Tare results in a subsequent read of approximately 0 g under the
  same load.
- **SC-004**: Set-known-weight produces a subsequent read within +/- 1 g of the
  desired value for weights between 0 g and 2500 g.
