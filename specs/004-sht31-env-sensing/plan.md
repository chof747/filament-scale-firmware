# Implementation Plan: Environmental Sensing Display & Metrics

**Branch**: `004-sht31-env-sensing` | **Date**: 2026-01-18 | **Spec**: [spec](./spec.md)
**Input**: Feature specification from `/specs/004-sht31-env-sensing/spec.md`

**Note**: This template is filled in by the `/speckit.plan` command. See `.specify/templates/commands/plan.md` for the execution workflow.

## Summary

Add periodic SHT31-D environmental sampling, emit temperature/humidity metrics every 60 seconds using the serial JSON line format, and extend the display cycle to show temperature then humidity (with unavailable indicators) using cached readings.

## Technical Context

**Language/Version**: C++ (Arduino framework) on PlatformIO
**Primary Dependencies**: Arduino core, Adafruit GFX, Adafruit SSD1306, HX711, SHT31-D sensor library
**Storage**: N/A (volatile readings only)
**Testing**: Manual device validation; optional lightweight checks in PlatformIO
**Target Platform**: Arduino Nano
**Project Type**: Single firmware project
**Performance Goals**: Display refresh within 2 seconds; periodic sampling/logging every 60 seconds
**Constraints**: Non-blocking loop, no dynamic allocation after setup, avoid `String`, Arduino Nano memory limits
**Scale/Scope**: Single device, single sensor

## Constitution Check

*GATE: Must pass before Phase 0 research. Re-check after Phase 1 design.*

- Clean, readable firmware: short functions, clear names, no dead code.
- Deterministic control flow: non-blocking loop, explicit timing/state machine.
- Arduino Nano limits: avoid dynamic allocation after setup and `String`; prefer fixed-size buffers.
- Arduino + PlatformIO first: keep Arduino conventions, PlatformIO config, and compile-time constants in `include/config.h`.
- Simple validation: no TDD requirement; tests optional and lightweight.
- Object-oriented firmware: hardware behavior encapsulated in classes.

**Post-Design Check**: PASS. No violations identified in the planned approach.

## Phase 0: Outline & Research

- Resolved display and logging decisions in `research.md` with rationale and alternatives.

## Phase 1: Design & Contracts

- Defined environmental reading data model in `data-model.md`.
- Documented log payload schema in `contracts/metrics-log-openapi.yaml`.
- Added validation steps in `quickstart.md`.

## Project Structure

### Documentation (this feature)

```text
specs/004-sht31-env-sensing/
├── plan.md              # This file (/speckit.plan command output)
├── research.md          # Phase 0 output (/speckit.plan command)
├── data-model.md        # Phase 1 output (/speckit.plan command)
├── quickstart.md        # Phase 1 output (/speckit.plan command)
├── contracts/           # Phase 1 output (/speckit.plan command)
└── tasks.md             # Phase 2 output (/speckit.tasks command - NOT created by /speckit.plan)
```

### Source Code (repository root)

```text
src/
include/
lib/
```

**Structure Decision**: Single firmware project using existing `src/`, `include/`, and `lib/` directories.

## Complexity Tracking

> **Fill ONLY if Constitution Check has violations that must be justified**

None.
# Implementation Plan: Environmental Sensing Display & Metrics

**Branch**: `004-sht31-env-sensing` | **Date**: 2026-01-18 | **Spec**: [spec](./spec.md)
**Input**: Feature specification from `/specs/004-sht31-env-sensing/spec.md`

**Note**: This template is filled in by the `/speckit.plan` command. See `.specify/templates/commands/plan.md` for the execution workflow.

## Summary

Add periodic SHT31-D environmental sampling, emit temperature/humidity metrics every 60 seconds using the serial JSON line format, and extend the display cycle to show temperature then humidity (with unavailable indicators) using cached readings.

## Technical Context

**Language/Version**: C++ (Arduino framework) on PlatformIO
**Primary Dependencies**: Arduino core, Adafruit GFX, Adafruit SSD1306, HX711, SHT31-D sensor library
**Storage**: N/A (volatile readings only)
**Testing**: Manual device validation; optional lightweight checks in PlatformIO
**Target Platform**: Arduino Nano
**Project Type**: Single firmware project
**Performance Goals**: Display refresh within 2 seconds; periodic sampling/logging every 60 seconds
**Constraints**: Non-blocking loop, no dynamic allocation after setup, avoid `String`, Arduino Nano memory limits
**Scale/Scope**: Single device, single sensor

## Constitution Check

*GATE: Must pass before Phase 0 research. Re-check after Phase 1 design.*

- Clean, readable firmware: short functions, clear names, no dead code.
- Deterministic control flow: non-blocking loop, explicit timing/state machine.
- Arduino Nano limits: avoid dynamic allocation after setup and `String`.
- Arduino + PlatformIO first: keep Arduino conventions, PlatformIO config, and compile-time constants in `include/config.h`.
- Simple validation: no TDD requirement; tests optional and lightweight.
- Object-oriented firmware: hardware behavior encapsulated in classes.

**Post-Design Check**: PASS. No violations identified in the planned approach.

## Phase 0: Outline & Research

- Resolved display and logging decisions in `research.md` with rationale and alternatives.

## Phase 1: Design & Contracts

- Defined environmental reading data model in `data-model.md`.
- Documented log payload schema in `contracts/metrics-log-openapi.yaml`.
- Added validation steps in `quickstart.md`.

## Project Structure

### Documentation (this feature)

```text
specs/004-sht31-env-sensing/
├── plan.md              # This file (/speckit.plan command output)
├── research.md          # Phase 0 output (/speckit.plan command)
├── data-model.md        # Phase 1 output (/speckit.plan command)
├── quickstart.md        # Phase 1 output (/speckit.plan command)
├── contracts/           # Phase 1 output (/speckit.plan command)
└── tasks.md             # Phase 2 output (/speckit.tasks command - NOT created by /speckit.plan)
```

### Source Code (repository root)

```text
src/
include/
lib/
```

**Structure Decision**: Single firmware project using existing `src/`, `include/`, and `lib/` directories.

## Complexity Tracking

> **Fill ONLY if Constitution Check has violations that must be justified**

None.
