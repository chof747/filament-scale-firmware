# Research: Scale sensing

## Decision

No additional research required. Feature is already implemented; the only
pending change is enforcing a 0 g to 2500 g upper bound for set-known-weight.

## Rationale

The codebase and agreed clarifications define behavior sufficiently for future
maintenance and the single planned change.

## Alternatives considered

- Adding performance benchmarks: deferred, not required for this change.
- Redesigning the interface: deferred to avoid altering existing behavior.
