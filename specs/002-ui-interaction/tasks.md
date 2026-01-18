# Tasks: UI interaction

**Input**: Design documents from `/specs/002-ui-interaction/`
**Prerequisites**: plan.md (required), spec.md (required for user stories), research.md, data-model.md, contracts/

**Tests**: Tests are OPTIONAL - only include them if explicitly requested in the feature specification.

**Organization**: Tasks are grouped by user story to enable independent implementation and testing of each story.

## Format: `[ID] [P?] [Story] Description`

- **[P]**: Can run in parallel (different files, no dependencies)
- **[Story]**: Which user story this task belongs to (e.g., US1, US2, US3)
- Include exact file paths in descriptions

## Phase 1: Documentation Sync (Existing Implementation)

**Purpose**: Align documentation with the existing implementation and agreed changes

- [ ] T001 Verify spec, data-model, and quickstart reflect the current UI behavior and agreed changes

---

## Phase 2: User Story 1 - View current scale on display (Priority: P1)

**Goal**: Ensure display updates include scale changes and startup rendering

**Independent Test**: Observe OLED after startup and after scale change with no weight delta

### Implementation for User Story 1

- [ ] T002 [US1] Force a display refresh on scale change in `src/main.cpp`
- [ ] T003 [US1] Render a weight once within 1 second of startup in `src/main.cpp`

---

## Phase 3: Validation

**Purpose**: Confirm UI behavior matches spec after changes

- [ ] T004 Validate OLED updates immediately after scale change even when weight delta is <1 g
- [ ] T005 Validate OLED shows a weight within 1 second of startup
- [ ] T006 Confirm short press and long press behaviors remain unchanged
