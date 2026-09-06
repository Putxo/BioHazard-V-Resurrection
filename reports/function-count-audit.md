# Function Count Audit — RE5DX9 1.1.0 Dev Release

Target SHA-256: `323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`

This audit supersedes both the provisional 71,993-candidate value and the earlier 79,108 single-pipeline audit.

## Result

**Reproducible high-confidence machine-code entrypoint denominator: 79,391**

This value was rebuilt from scratch using two independent x86 disassemblers:

- GNU `objdump`
- LLVM `llvm-objdump`

Only address classes supported by both decoders and by an additional function-entry signal are accepted.

## Dual-decoder base

The two disassemblers agree on:

- **72,919** 16-byte-aligned instruction starts immediately after compiler `INT3` padding.
- **19,061** valid direct `CALL` targets in `.text`.
- Of those CALL targets, **1,832** are new entries outside the padding-boundary set.
- The PE entrypoint contributes **1** additional entry.

Deduplicated base union: **74,752**.

The prior 79,108 audit used 72,565 padding starts. A clean recount showed that value was not stable across independent decoding and therefore could not remain the canonical denominator.

## Indirect entry evidence

Additional entries are admitted only when the target is an instruction boundary in both disassemblers, is 16-byte aligned, and the instruction immediately before the target is a `RET` ending at that address in both decoders.

Using that rule:

- **4,336** additional targets are referenced as aligned 32-bit code pointers from `.rdata` / `.data` and are not already in the base set.
- **250** additional targets are explicitly materialized as immediate callback/function addresses by `mov`/`push`-class instructions and are not already covered by the base/data-pointer sets.
- **53** additional targets are reached only through direct `JMP` / tail-entry references and are not already covered by the preceding sets.

After deduplication:

`74,752 + 4,336 + 250 + 53 = 79,391`

## Independent RTTI cross-check

The same executable contains:

- 433 MSVC TypeDescriptors
- 494 CompleteObjectLocators
- 494 vtables
- 3,534 virtual slots
- 1,682 unique virtual method targets

These structures are used as an independent cross-check that virtual methods are represented in the accepted code-entry set.

## Why 79,108 was rejected

A full clean recount produced discrepancies with the earlier audit:

- GNU and LLVM disagree on only a handful of individual linear-disassembly boundaries, so the audit now uses their intersection rather than trusting one decoder.
- The previous 72,565 padding-start count did not reproduce; the common, aligned compiler-padding count is 72,919.
- Tail-call/JMP-only entries had not been separated as their own evidence class.
- Immediate-address parsing previously allowed operand forms that could mistake memory displacements/constants for function addresses; V3 only accepts strict immediate-address forms.

Therefore **79,108 is not considered verified and must not be used as the canonical denominator**.

## Important limitation

The PE is stripped of COFF function symbols and contains no embedded authoritative function-boundary table. Its debug directory contains only an RSDS reference to:

`BH5DCRelease.pdb`

GUID: `b18cf408-7751-4239-b0a1-85d621aeaf94`
Age: `23`

The matching PDB/MAP is not available in the user's Library and exact public searches for the filename/GUID produced no result.

Consequently, **79,391 is the reproducible high-confidence machine-code entrypoint denominator defined by the project's V3 acceptance rule. It is not a proof that the original C/C++ source contained exactly 79,391 source-level functions.** A symbol-perfect source-function count requires the matching original PDB/MAP or equivalent linker metadata.

## Policy

The dashboard denominator is **79,391**. Any future revision requires:

1. a new baseline ID;
2. a concrete address-level delta;
3. the same dual-decoder verification;
4. updated reports/README;
5. CI PASS before merge.
