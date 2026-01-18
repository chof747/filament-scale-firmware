# Tasks: Scale sensing

**Input**: Design documents from `/specs/001-scale-sensing/`
**Prerequisites**: plan.md (required), spec.md (required for user stories), research.md, data-model.md, contracts/

**Tests**: Tests are OPTIONAL - only include them if explicitly requested in the feature specification.

**Organization**: Tasks are grouped by user story to enable independent implementation and testing of each story.

## Format: `[ID] [P?] [Story] Description`

- **[P]**: Can run in parallel (different files, no dependencies)
- **[Story]**: Which user story this task belongs to (e.g., US1, US2, US3)
- Include exact file paths in descriptions

## Phase 1: Documentation Sync (Existing Implementation)

**Purpose**: Align documentation with the existing implementation and agreed changes

- [ ] T001 Update spec and data-model docs to reflect the new set-known-weight upper bound change (if needed)
- [ ] T002 Verify plan, research, and quickstart docs reflect current behavior and the agreed change

---

## Phase 2: User Story 3 - Set known weight (Priority: P3)

**Goal**: Enforce the 0 g to 2500 g upper bound for set-known-weight

**Independent Test**: Invoke set-known-weight with 0 g, 2500 g, and 2501 g and verify expected accept/reject behavior

### Implementation for User Story 3

- [ ] T003 [US3] Add upper-bound validation for set-known-weight in `src/scale.cpp`
- [ ] T004 [US3] Update any related logging or error paths to reflect out-of-range rejection

---

## Phase N: Validation

**Purpose**: Confirm behavior matches spec after change

- [ ] T005 Validate set-known-weight accepts 0 g to 2500 g and rejects >2500 g on hardware
- [ ] T006 Confirm no changes to read/tare behaviors
