# Function Inventory — Phase 11 methodology

The exact target PDB is not available; only its CodeView path remains in the PE. Therefore this inventory does not pretend to contain original symbols.

## Detection rules

A candidate function start is enrolled only when it is a valid decoded instruction inside `.text` and at least one of these is true:

1. it is the PE entry point;
2. it is the destination of a decoded direct `CALL` from `.text`;
3. it immediately follows a run of at least two `INT3` (`0xCC`) padding bytes.

Arbitrary prologue-looking byte patterns are deliberately excluded. Arbitrary JMP targets are excluded because they overwhelmingly represent intra-function basic blocks. Arbitrary pointers from `.rdata/.data` are also excluded at this stage because switch/jump tables create basic-block false positives; those references are handled in Phase 12 with context.

## Confidence

- `CONFIRMED`: PE entry point.
- `HIGH`: both padding and direct-call evidence, or at least two direct-call references.
- `MEDIUM`: one accepted evidence family only.

Function size is provisional: the span to the next enrolled start with trailing INT3 padding trimmed. It is explicitly an estimated size until later control-flow validation.

## Important

The Phase 11 count is a versioned canonical candidate count, not an assertion that every boundary is final. Phase 12 and the Inventory Freeze may correct it with evidence, and the dashboard denominator must track those corrections rather than preserving a stale number.
