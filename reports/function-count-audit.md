# Function Count Audit — RE5DX9 1.1.0 Dev Release

Target SHA-256: `323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`

This V5 audit supersedes V4's 79,493 working denominator.

## Current result

**V5 high-confidence machine-code entrypoint denominator: 79,016**

V5 differs from V4 because it no longer lets references produced by linearly disassembled but CFG-unreachable bytes create function roots.

## Whole executable

The entire 19,977,216-byte PE is accounted for by headers plus `.text`, `.rdata`, `.data` and `.rsrc`, with zero raw-file bytes left unclassified.

The complete `.text` section was disassembled with both:

- GNU `objdump` 2.44
- LLVM `llvm-objdump` 17.0.0

Instruction-start totals:

- GNU: **4,836,558**
- LLVM: **4,836,307**
- common starts: **4,835,646**
- GNU-only: 912
- LLVM-only: 661

## Fixed-point entry derivation

V5 starts from strong roots and iterates control flow until no new root appears.

- 16-byte-aligned common starts immediately after `INT3`: **72,919**
- reachable direct CALL targets: **16,571**
- CALL additions beyond the padding set: **1,669**
- PE-entry-only addition: **1**
- direct/padding base union: **74,589**
- strong `.rdata` / `.data` address-taken additions: **4,338**
- strict reachable immediate callback additions: **45**
- strong reachable tail-entry additions: **44**

Final deduplicated union:

**79,016**

The graph reaches the same root set on the second iteration: **0 new roots**, so the V5 rule has converged.

## Reachability

- reachable common instructions: **3,726,072**
- reachable instruction bytes: **13,772,419**
- `.text` virtual bytes: **16,185,403**
- reachable-code coverage: **85.09%**
- non-reached `.text` bytes: **2,412,984**

Those non-reached bytes are not automatically code. They include padding, zero-filled areas, embedded tables/constants, decoder-disagreement regions and possibly some dead/unreferenced code.

## RTTI cross-check

- TypeDescriptors: **433**
- CompleteObjectLocators: **494**
- structural vtables: **494**
- virtual slots: **3,534**
- unique virtual targets: **1,682**

## Why V4 was reduced by 477

V4 accepted **79,493** high-confidence entries. V5 removes **477** from that high-confidence denominator because their only supporting references came from linearly decoded areas that were not reached by the CFG and/or from looser immediate/table parsing. They may be retained in an ambiguous/unproven pool, but they no longer count as completed inventory roots.

## Exact source-function count remains unavailable

The PE has zero COFF function symbols and no authoritative embedded all-function boundary table. It only contains an RSDS reference to `BH5DCRelease.pdb` (GUID `b18cf408-7751-4239-b0a1-85d621aeaf94`, age 23). The matching PDB/MAP is unavailable.

Therefore **79,016 is a reproducible high-confidence machine-entry denominator, not proof that the original C/C++ source contained exactly 79,016 functions**.

See `reports/full-executable-reaudit-v5.md` for the full audit.
