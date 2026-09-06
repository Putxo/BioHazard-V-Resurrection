# GATE 02 — Target Definition

Phase: 2 — Target Definition

## Requirements

- [x] `config/target.json` contains only known identity-level facts and null/UNKNOWN for undiscovered binary-specific fields.
- [x] A local-only PE32 target verifier exists.
- [x] The verifier can validate MZ/PE signatures, architecture form, SHA-256, size, timestamp, image base and entry point when a local binary is supplied.
- [x] No original executable is stored in Git.
- [x] Project state records one active coordinator and locks later phases.

## Important limitation

No local target executable is available through the repository, by design. Therefore binary-derived target fields remain unknown until the Binary Inventory phase receives a user-supplied local executable.

Result: PASS
