# GATE 05 — Validation Core

Phase: 5 — Validation Core

## Requirements

- [x] Function stable-ID duplication is rejected.
- [x] Duplicate function VAs are rejected.
- [x] Overlapping known function ranges are rejected.
- [x] Invalid status values are rejected.
- [x] VERIFIED/MATCHED/LINKED records require evidence.
- [x] Duplicate active claims are rejected.
- [x] Common proprietary binary/material extensions are rejected.
- [x] Suspicious files over 20 MiB are rejected for manual review.
- [x] Validation helpers have explicit unit-test cases.

## Validation note

The core identity/evidence helper logic was executed independently before commit with empty, duplicate-ID and missing-evidence cases; all assertions passed. CI integration is intentionally deferred to Phase 8.

Result: PASS
