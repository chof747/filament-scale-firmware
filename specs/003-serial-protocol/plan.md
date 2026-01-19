# Implementation Plan: Serial protocol

**Branch**: `[003-serial-protocol]` | **Date**: 2026-01-17 | **Spec**: specs/003-serial-protocol/spec.md
**Input**: Feature specification from `/specs/003-serial-protocol/spec.md`

**Note**: Feature is already implemented. This plan focuses on documentation and
agreed changes (fix metric JSON formatting).

## Summary

Serial protocol is implemented via JSON log/metric output and simple set-weight
commands. The plan documents current behavior and applies the agreed fix for
metric JSON formatting.

## Technical Context

**Language/Version**: C++ (Arduino framework)  
**Primary Dependencies**: Arduino core  
**Storage**: N/A  
**Testing**: PlatformIO unit tests (optional)  
**Target Platform**: Arduino Nano  
**Project Type**: single firmware project  
**Performance Goals**: low-latency serial output, non-blocking command parsing  
**Constraints**: low memory, avoid dynamic allocation after setup  
**Scale/Scope**: single device serial interface

## Constitution Check

*GATE: Must pass before Phase 0 research. Re-check after Phase 1 design.*

- Clean, readable firmware: short functions, clear names, no dead code.
- Deterministic control flow: non-blocking loop, explicit timing/state machine.
- Arduino Nano limits: avoid dynamic allocation after setup and `String`; prefer fixed-size buffers.
- Arduino + PlatformIO first: keep Arduino conventions, PlatformIO config, and
  compile-time constants in `include/config.h`.
- Simple validation: no TDD requirement; tests optional and lightweight.
- Object-oriented firmware: hardware behavior encapsulated in classes.

Status: PASS

## Project Structure

### Documentation (this feature)

```text
specs/003-serial-protocol/
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
├── main.cpp
├── commands.cpp
├── logging.cpp
├── scale.cpp
└── config.cpp

include/
├── commands.h
├── logging.h
├── scale.h
└── config.h
```

**Structure Decision**: Single firmware project; serial logic in `src/commands.cpp`
and `src/logging.cpp`.

## Complexity Tracking

> **Fill ONLY if Constitution Check has violations that must be justified**

None.
