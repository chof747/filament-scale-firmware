# Implementation Plan: Scale sensing

**Branch**: `[001-scale-sensing]` | **Date**: 2026-01-17 | **Spec**: specs/001-scale-sensing/spec.md
**Input**: Feature specification from `/specs/001-scale-sensing/spec.md`

**Note**: Feature is already implemented. This plan focuses on documentation and
agreed changes (set-known-weight upper bound).

## Summary

Scale sensing is implemented via the FilamentScale class. The plan documents
current behavior and adds the agreed change to enforce the 0 g to 2500 g upper
bound for set-known-weight.

## Technical Context

**Language/Version**: C++ (Arduino framework)  
**Primary Dependencies**: Arduino core, HX711 library  
**Storage**: N/A  
**Testing**: PlatformIO unit tests (optional)  
**Target Platform**: Arduino Nano  
**Project Type**: single firmware project  
**Performance Goals**: deterministic loop, sub-second readiness checks  
**Constraints**: low memory, avoid dynamic allocation after setup  
**Scale/Scope**: 3 scales, single device

## Constitution Check

*GATE: Must pass before Phase 0 research. Re-check after Phase 1 design.*

- Clean, readable firmware: short functions, clear names, no dead code.
- Deterministic control flow: non-blocking loop, explicit timing/state machine.
- Arduino Nano limits: avoid dynamic allocation after setup and `String`; prefer fixed-size buffers.
- Arduino + PlatformIO first: keep Arduino conventions and PlatformIO config.
- Simple validation: no TDD requirement; tests optional and lightweight.
- Object-oriented firmware: hardware behavior encapsulated in classes.

Status: PASS

## Project Structure

### Documentation (this feature)

```text
specs/001-scale-sensing/
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
├── scale.cpp
├── config.cpp
├── commands.cpp
└── logging.cpp

include/
├── scale.h
├── config.h
├── commands.h
└── logging.h
```

**Structure Decision**: Single firmware project; feature logic in `src/scale.cpp`
with headers in `include/`.

## Complexity Tracking

> **Fill ONLY if Constitution Check has violations that must be justified**

None.
