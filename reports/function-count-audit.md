# Function Count Audit — RE5DX9 1.1.0 Dev Release

Target SHA-256: `323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`

This V4 audit supersedes the provisional 71,993 count, the 79,108 single-pipeline count and the V3 79,391 denominator.

## Result

**Current high-confidence machine-code entrypoint working denominator: 79,493**

**Dual-decoder base set: 74,752**

The complete executable was re-analyzed from scratch with:

- GNU `objdump` 2.44
- LLVM `llvm-objdump` 17.0.0

Every `.text` region was processed. No sample-only region or subsystem estimate is used.

## Complete instruction agreement

- GNU instruction starts: **4,836,558**
- LLVM instruction starts: **4,836,307**
- instruction starts agreed by both: **4,835,646**
- GNU-only starts: **912**
- LLVM-only starts: **661**

Common control-flow targets inside `.text`:

- direct CALL targets: **19,061**
- direct JMP targets: **35,878**
- conditional-branch targets: **187,062**

Compiler-padding boundaries:

- common starts after any `INT3` run: **80,494**
- common starts after `INT3` that are 16-byte aligned: **72,919**

## Base entry set

V4 keeps a conservative independently evidenced base:

- 16-byte-aligned post-`INT3` starts agreed by both decoders
- direct CALL targets agreed by both decoders
- PE entrypoint

After deduplication: **74,752**.

## Exact MSVC RTTI/vtable cross-check

Recounted directly from the PE:

- TypeDescriptors: **433**
- CompleteObjectLocators: **494**
- structural vtables: **494**
- virtual slots: **3,534**
- unique virtual targets: **1,682**
- structural vtable targets outside the base set: **108**

Two virtual slots point to addresses in the tiny GNU/LLVM decoder-disagreement set. The structural RTTI/vtable evidence remains authoritative for those address-taken virtual entries; this explains why a prior decoder-filtered pass temporarily reported 493 vtables / 3,528 slots.

## Extended high-confidence entries

Outside the base set, V4 accepts an address-taken/control-flow entry only when there is independent boundary evidence. The main static rule requires the target to begin immediately after a `RET` boundary in both decoders; conditional-branch labels are excluded from this extension. Exact structural RTTI vtable entries are also retained.

Sequential deduplicated additions used by V4:

- address-taken table entries with dual-RET boundary: **4,344**
- immediate callback/function-address entries with dual-RET boundary: **323**
- direct JMP/tail entries with dual-RET boundary: **68**
- exact structural vtable targets still outside that extended union: **6**

Final high-confidence working set: **79,493**.

## Why 79,391 is rejected as exact

The full V4 pass does not reproduce the V3 79,391 union. V3 depended on a narrower parsing/filtering combination for pointer-table, immediate-address and tail-entry evidence. Re-running the entire executable with one consistent dual-decoder rule produces a different address-level union.

Therefore **79,391 must not be described as a verified exact total**.

## What can and cannot be verified

The PE contains:

- zero COFF symbols;
- no authoritative all-function boundary table;
- only an RSDS reference to `BH5DCRelease.pdb`;
- PDB GUID `b18cf408-7751-4239-b0a1-85d621aeaf94`, age 23.

The matching PDB/MAP is unavailable.

Consequently:

- **74,752** is the V4 dual-decoder base machine-entry set.
- **79,493** is the current V4 high-confidence machine-code entrypoint working denominator.
- The exact number of original C/C++ source-level functions **cannot be proven from this stripped executable alone**.

A symbol-perfect source-function count requires the matching original PDB/MAP or equivalent linker metadata.

## Whole-executable coverage

- `.text` virtual size: **16,185,403 bytes**
- 64-KiB regions audited: **247 / 247**
- regions with zero common decoded instructions: **0**
- regions with zero base entries: **0**

See `reports/full-executable-reaudit-v4.md` for PE sections, instruction totals, imports, strings and regional coverage.

## Policy

The dashboard uses **79,493** as the current high-confidence decompilation working denominator, but documentation must always state that it is not a symbol-perfect source-function count. Any future revision requires a complete evidence-backed audit and CI PASS before merge.
