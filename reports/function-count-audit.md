# Function Count Audit — RE5DX9 1.1.0 Dev Release

Target SHA-256: `323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`

## Current dashboard denominator

**Current canonical dashboard universe: 79,782 potential function starts**

The dashboard denominator is the raw V6 union represented by the canonical local `work/analysis/function_candidates.csv`. That CSV is intentionally not tracked in Git because it is a large local analysis artifact, so denominator corrections must distinguish two independent questions:

1. whether an address is actually a row in `function_candidates.csv`; and
2. if it is a row, whether later binary analysis proves that row is not a real function entry.

Only an address satisfying (1) can ever be subtracted from the 79,782-row denominator.

## 2026-09-24 membership-audit repair

A local exact membership check found that the following addresses are **not present** in `function_candidates.csv`:

- `0x004070E0`
- `0x00407A10`
- `0x00407A30`
- `0x00407AB0`
- `0x00407AD0`
- `0x00407AF0`
- `0x00407B60`
- `0x00407BA0`
- `0x00407BE0`
- `0x00407C70`
- `0x00407C90`
- `0x00407CB0`

Among the recently worked addresses, only `0x00407B10` and `0x00407C00` are rows in that CSV.

Nine of the absent addresses — `0x00407A10`, `0x00407AB0`, `0x00407AD0`, `0x00407BA0`, `0x00407B60`, `0x00407BE0`, `0x00407C70`, `0x00407C90`, and `0x00407CB0` — had incorrectly been counted as evidence-backed false-positive removals. That was a bookkeeping error: their binary classifications may still be useful, but they were never members of the 79,782-row candidate universe and therefore cannot reduce it.

The earlier sequence of dashboard reductions from 79,782 down to 79,773 is superseded. The canonical denominator is restored to **79,782**, with **zero classified false-positive removals from the V6 CSV** at this point.

## Recovered functions that are not V6 CSV rows

Absence from `function_candidates.csv` does not prove that an address is not a real function. The candidate CSV is a detector output, not an authoritative source-symbol table.

Accordingly, valid recovered implementations, tests, evidence, claims/history and validation metadata are preserved. In particular, the existing recoveries for `FUN_00407A30` and `FUN_00407AF0` remain valid despite those addresses not being V6 CSV rows. The valid recent recoveries for `FUN_00407B10` and `FUN_00407C00` are also preserved; those two addresses do appear in the CSV.

No source code or test is deleted by this accounting repair.

## Independent full-scan evidence

The V6 static pass over the exact target produced:

- complete `.text` linear scan rows: **4,837,889**
- undecodable bytes explicitly retained as data: **662**
- semantic direct `CALL` targets inside code: **19,195**
- targets in aligned sequences of at least two code pointers: **40,912**
- 16-byte-aligned starts after at least two `INT3`/`NOP` padding bytes: **69,076**

Deduplicated union of those candidate-start evidence classes:

**79,782**

## V5 fixed-point cross-check

The earlier V5 fixed-point CFG method remains a stricter subset/cross-check:

- V5 high-confidence machine-code entries: **79,016**
- V6 broader candidate union: **79,782**
- difference: **766** candidates

Neither number is presented as an exact count of original source-level C/C++ functions. The target has no authoritative embedded all-function symbol table and the matching PDB/MAP is unavailable.

## Supplemental address analyses

On public `main`, the eight previously published address-analysis files through `0x00407C90` are retained for their byte-level disassembly and control-flow findings and reclassified as **supplemental address analyses**, not candidate removals. The attempted `0x00407CB0` publication PR #320 was closed without merge, so this repair does not add its evidence file to `main`. None of these analyses may alter the denominator unless a future audit first proves that the analyzed address is actually a row in the canonical `function_candidates.csv`.

## Accounting policy going forward

For project accounting, **79,782 is the canonical candidate denominator**.

A future denominator reduction requires both:

- exact membership evidence showing the address is a row in the canonical local `function_candidates.csv`; and
- versioned binary evidence showing that CSV row is not a real function entry.

This two-part check prevents a correct binary classification of an arbitrary address from being mistaken for removal of a candidate that never existed in the V6 universe.
