# GATE 10 — Binary Inventory

Phase: 10 — Binary Inventory

## Input identity

- Local-only target SHA-256: `323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`
- Size: `19,977,216` bytes
- The executable was obtained from the user's existing file library and materialized only into the analysis workspace. It is not committed.

## Requirements

- [x] Exact target hash and file size confirmed.
- [x] MZ/PE32/i386 identity confirmed.
- [x] PE timestamp, ImageBase and entry point inventoried.
- [x] Section table inventoried with VA/RVA/raw bounds.
- [x] PE data directories inventoried.
- [x] Import surface inventoried: 21 DLLs / 376 imported names or ordinals.
- [x] Debug CodeView/RSDS record inventoried.
- [x] Toolchain clues recorded without inventing an exact compiler version.
- [x] Relocation/TLS/exception directory absence recorded.
- [x] Deterministic local PE inventory tooling added.
- [x] No executable, DLL or game asset committed.
- [x] No function count fabricated.

Result: PASS

Phase 11 — Function Inventory is the next unlocked phase.
