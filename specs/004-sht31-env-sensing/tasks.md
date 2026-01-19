---

description: "Task list for Environmental Sensing Display & Metrics"
---

# Tasks: Environmental Sensing Display & Metrics

**Input**: Design documents from `/specs/004-sht31-env-sensing/`
**Prerequisites**: plan.md (required), spec.md (required for user stories), research.md, data-model.md, contracts/

**Tests**: Not requested; no automated test tasks included.

**Organization**: Tasks are grouped by user story to enable independent implementation and testing of each story.

## Format: `[ID] [P?] [Story] Description`

- **[P]**: Can run in parallel (different files, no dependencies)
- **[Story]**: Which user story this task belongs to (e.g., US1, US2, US3)
- Include exact file paths in descriptions

## Phase 1: Setup (Shared Infrastructure)

**Purpose**: Project initialization and shared configuration

- [X] T001 Define environmental sensor constants (I2C address, logging interval, display tokens) in `include/config.h`
- [X] T002 Declare the SHT31 library dependency in `platformio.ini`

---

## Phase 2: Foundational (Blocking Prerequisites)

**Purpose**: Core infrastructure that MUST be complete before ANY user story can be implemented

**⚠️ CRITICAL**: No user story work can begin until this phase is complete

- [X] T003 [P] Add SHT31 sensor interface in `include/sht31.h`
- [X] T004 [P] Implement SHT31 sensor sampling and cached reading in `src/sht31.cpp`
- [X] T005 Wire SHT31 class into firmware setup and loop timing in `src/main.cpp`
- [X] T006 Define metric/log JSON line helpers compatible with serial protocol in `include/logging.h`
- [X] T007 Implement metric/log JSON line output in `src/logging.cpp`

**Checkpoint**: Foundation ready - user story implementation can now begin in parallel

---

## Phase 3: User Story 1 - View Environment Readings in Cycle (Priority: P1) 🎯 MVP

**Goal**: Show temperature then humidity screens after the last scale using cached readings

**Independent Test**: Cycle with the mode button and verify temperature then humidity appear before returning to scale 0

### Implementation for User Story 1

- [X] T008 [US1] Add environment display states to the cycle in `include/scale.h`
- [X] T009 [US1] Update display cycle to include temperature then humidity screens in `src/scale.cpp`
- [X] T010 [P] [US1] Format labeled temperature/humidity display strings in `src/scale.cpp`

**Checkpoint**: User Story 1 is fully functional and testable independently

---

## Phase 4: User Story 2 - Receive Environment Metrics Logs (Priority: P2)

**Goal**: Emit periodic temperature and humidity metric JSON lines

**Independent Test**: Observe serial output for metric JSON lines every 60 seconds

### Implementation for User Story 2

- [X] T011 [US2] Emit temperature and humidity metric messages every interval in `src/main.cpp`
- [X] T012 [US2] Populate metric fields (type, ts_ms, sensor, measurement, value, unit) via helpers in `src/logging.cpp`

**Checkpoint**: User Stories 1 and 2 are both independently functional

---

## Phase 5: User Story 3 - Handle Sensor Read Issues Transparently (Priority: P3)

**Goal**: Show unavailable indicators and emit WARN logs on read failures

**Independent Test**: Simulate a sensor failure and confirm unavailable display plus WARN log line

### Implementation for User Story 3

- [X] T013 [US3] Track read validity and failure state in `include/sht31.h`
- [X] T014 [US3] Update unavailable indicator handling in `src/scale.cpp`
- [X] T015 [US3] Emit WARN log line on read failure in `src/main.cpp`
- [X] T016 [US3] Emit metric lines with `value = null` and no unit when unavailable in `src/main.cpp`
- [X] T017 [US3] Detect missing sensor at startup and mark readings unavailable in `src/sht31.cpp`
- [X] T018 [US3] Handle out-of-range readings by marking them unavailable in `src/sht31.cpp`
- [X] T019 [US3] Ensure rapid cycling does not trigger blocking reads in `src/scale.cpp`

**Checkpoint**: All user stories are independently functional

---

## Phase 6: Polish & Cross-Cutting Concerns

**Purpose**: Improvements that affect multiple user stories

- [ ] T020 Run the validation steps in `specs/004-sht31-env-sensing/quickstart.md`

---

## Dependencies & Execution Order

### Phase Dependencies

- **Setup (Phase 1)**: No dependencies - can start immediately
- **Foundational (Phase 2)**: Depends on Setup completion - BLOCKS all user stories
- **User Stories (Phase 3+)**: All depend on Foundational phase completion
  - User stories can then proceed in parallel (if staffed)
  - Or sequentially in priority order (P1 → P2 → P3)
- **Polish (Final Phase)**: Depends on all desired user stories being complete

### User Story Dependencies

- **User Story 1 (P1)**: Can start after Foundational (Phase 2) - No dependencies on other stories
- **User Story 2 (P2)**: Can start after Foundational (Phase 2) - Uses logging helpers from Foundation
- **User Story 3 (P3)**: Can start after Foundational (Phase 2) - Builds on sensor validity state

### Parallel Opportunities

- T003 and T004 can run in parallel (header/implementation split)
- User Story phases can run in parallel after Phase 2 (if team capacity allows)

---

## Parallel Example: User Story 1

```bash
Task: "Add environment display states to the cycle in include/scale.h"
Task: "Format labeled temperature/humidity display strings in src/scale.cpp"
```

## Parallel Example: User Story 2

```bash
Task: "Emit temperature and humidity metric messages every interval in src/main.cpp"
Task: "Populate metric fields via helpers in src/logging.cpp"
```

## Parallel Example: User Story 3

```bash
Task: "Track read validity and failure state in include/sht31.h"
Task: "Update unavailable indicator handling in src/scale.cpp"
```

---

## Implementation Strategy

### MVP First (User Story 1 Only)

1. Complete Phase 1: Setup
2. Complete Phase 2: Foundational (CRITICAL - blocks all stories)
3. Complete Phase 3: User Story 1
4. **STOP and VALIDATE**: Test User Story 1 independently

### Incremental Delivery

1. Complete Setup + Foundational → Foundation ready
2. Add User Story 1 → Test independently → Demo
3. Add User Story 2 → Test independently → Demo
4. Add User Story 3 → Test independently → Demo
5. Run Polish tasks

---

## Notes

- [P] tasks = different files, no dependencies
- [Story] label maps task to specific user story for traceability
- Each user story is independently completable and testable
