<!--
Sync Impact Report
- Version change: 0.1.0 -> 0.2.0
- Modified principles: None
- Added sections: None
- Removed sections: None
- Added principles: Object-Oriented Firmware Structure
- Templates requiring updates:
  - .specify/templates/plan-template.md ✅ updated
  - .specify/templates/spec-template.md ✅ updated (no change required)
  - .specify/templates/tasks-template.md ✅ updated (no change required)
  - .specify/templates/commands/*.md ⚠ pending (directory missing)
- Follow-up TODOs: None
-->
# filament-scale-firmware Constitution

## Core Principles

### Clean, Readable Firmware
Firmware MUST remain small, readable, and easy to reason about on-device. Use
clear names, short functions, single-responsibility modules, and avoid dead
code. Comments are reserved for non-obvious hardware rationale or timing
constraints, not restating code.

Rationale: A tiny MCU benefits most from clarity; readability prevents subtle
hardware regressions.

### Deterministic Control Flow
The main loop MUST be deterministic and non-blocking. Use finite state
machines, explicit timing via `millis()`, and debounced inputs. Avoid
long `delay()` calls and keep ISR work minimal.

Rationale: Deterministic flow prevents missed readings and jitter.

### Arduino Nano Resource Limits
Code MUST respect Arduino Nano limits (flash, SRAM, CPU). Avoid dynamic
allocation after setup, prefer fixed-size buffers, avoid `String`, and place
static lookup data in `PROGMEM` where appropriate.

Rationale: Resource spikes cause instability and resets on constrained MCUs.

### Arduino + PlatformIO First
The firmware MUST use Arduino framework conventions and PlatformIO build
configuration. Keep hardware-facing code in `src/` and shared headers in
`include/`. Avoid OS-specific dependencies and non-embedded abstractions.

Rationale: Consistent platform usage keeps builds predictable and portable.

### Simple Validation, No TDD
Test-driven development is NOT required. Tests are optional and should stay
lightweight; prefer manual validation on hardware, serial logs, and focused
checks that can run within PlatformIO when beneficial.

Rationale: The firmware is small and hardware-bound, so pragmatic validation
is sufficient.

### Object-Oriented Firmware Structure
Firmware MUST use classes to encapsulate hardware behavior and state, and keep
object responsibilities clear. Procedural code is acceptable only for glue
logic around object boundaries.

Rationale: Encapsulation keeps hardware interactions consistent and maintainable.

## Hardware & Performance Constraints

- Target platform is Arduino Nano; design for low memory usage and fast loop
  execution.
- Avoid blocking I/O; use non-blocking reads and simple timers.
- Prefer compile-time configuration and constants over dynamic configuration.
- Keep serial logging concise; disable verbose logging in release builds.

## Development Workflow

- Use PlatformIO for builds and uploads; keep `platformio.ini` authoritative.
- Every change MUST be checked against the Core Principles.
- When touching hardware pin mappings or timing, update documentation or inline
  rationale comments.
- Keep changes scoped; avoid multi-feature refactors unless required.

## Governance

- This constitution supersedes other guidance; conflicts must be resolved in
  favor of the constitution.
- Amendments require an explicit update to this file, rationale in the change
  description, and reviewer acknowledgment.
- Versioning follows SemVer: MAJOR for removals/redefinitions, MINOR for new
  principles/sections, PATCH for clarifications.
- Reviews MUST verify compliance with the Core Principles and constraints.

**Version**: 0.2.0 | **Ratified**: 2026-01-17 | **Last Amended**: 2026-01-17
