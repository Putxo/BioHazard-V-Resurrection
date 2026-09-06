# Function Count Audit — RE5DX9 1.1.0 Dev Release

Target SHA-256: `323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`

This audit replaces the earlier provisional 71,993-candidate denominator.

## Result

**Audited machine-code function entrypoints: 79,108**

Breakdown:

- 72,565 instruction starts immediately after compiler `INT3` padding boundaries.
- 1,833 additional unique direct `CALL` targets not already represented by those padded starts.
- 1 PE entrypoint not contained in the previous two sets.
- Base union: 74,399 machine-code entries.
- 4,378 additional boundary-valid targets recovered from function-pointer-table runs after separating switch-like/ambiguous tables from function-like tables.
- 331 additional boundary-valid callbacks/function entries whose text addresses are materialized explicitly as immediate operands in code and are not already present in the preceding sets.
- Final deduplicated union: **79,108**.

## RTTI cross-check

The exact MSVC RTTI structures independently recovered from the same executable are:

- 433 TypeDescriptors
- 494 CompleteObjectLocators
- 494 vtables
- 3,534 virtual slots
- 1,682 unique virtual method targets
- 108 virtual method entrypoints exist outside the padding/direct-CALL base union; these are included in the audited total.

## Why this is stronger than the previous 71,993 value

The previous number mixed heuristic function candidates and was intentionally provisional. This audit uses independent entry evidence: compiler padding boundaries, actual direct call targets, PE entrypoint, validated function-pointer-table behavior, explicit function-address materialization, and MSVC RTTI/vtables.

## Important limitation

This PE does not contain COFF function symbols or an embedded function-boundary table. Its debug directory contains only an RSDS reference to:

`BH5DCRelease.pdb`

GUID: `b18cf408-7751-4239-b0a1-85d621aeaf94`
Age: `23`

The matching PDB is not present in the available Library, and an exact filename/GUID web search did not locate a public copy. Therefore **79,108 is the audited machine-code entrypoint count used for decompilation progress; it is not claimed to be the exact original source-symbol function count**. A symbol-perfect count would require the matching PDB/MAP or equivalent original linker metadata.

## Policy

The dashboard denominator is now 79,108. Any future change must be introduced as a new baseline revision with concrete evidence and a documented delta. PILOT/SCALE must not silently alter the denominator.
