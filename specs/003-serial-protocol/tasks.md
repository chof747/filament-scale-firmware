# Tasks: Serial protocol

**Input**: Design documents from `/specs/003-serial-protocol/`
**Prerequisites**: plan.md (required), spec.md (required for user stories), research.md, data-model.md, contracts/

**Tests**: Tests are OPTIONAL - only include them if explicitly requested in the feature specification.

**Organization**: Tasks are grouped by user story to enable independent implementation and testing of each story.

## Format: `[ID] [P?] [Story] Description`

- **[P]**: Can run in parallel (different files, no dependencies)
- **[Story]**: Which user story this task belongs to (e.g., US1, US2, US3)
- Include exact file paths in descriptions

## Phase 1: Documentation Sync (Existing Implementation)

**Purpose**: Align documentation with the existing implementation and agreed changes

- [ ] T001 Verify spec, data-model, and quickstart reflect current serial behavior and agreed changes

---

## Phase 2: User Story 1 - Receive weight metrics (Priority: P1)

**Goal**: Ensure metric JSON is well-formed

**Independent Test**: Observe serial output and validate JSON lines parse correctly

### Implementation for User Story 1

- [ ] T002 [US1] Fix metric JSON formatting in `src/logging.cpp`

---

## Phase 3: Validation

**Purpose**: Confirm serial output matches spec after change

- [ ] T003 Validate metric JSON lines parse correctly in a host parser
- [ ] T004 Confirm log output remains unchanged
- [ ] T005 Confirm command handling still works for valid and invalid inputs
