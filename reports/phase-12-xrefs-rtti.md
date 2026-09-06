# Phase 12 — XREF / RTTI / vtable inventory

Target: `RE5DX9 1.1.0 Dev (Release 15 Dec 2017)`  
SHA-256: `323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`

## Confirmed structural inventory

| Evidence | Count |
|---|---:|
| MSVC RTTI TypeDescriptors | 433 |
| MSVC CompleteObjectLocators | 494 |
| Confirmed vtables | 494 |
| Virtual slots across confirmed vtables | 3,534 |
| Unique virtual-method targets | 1,682 |
| aligned `.rdata` pointers into `.text` | 207,370 |
| unique `.rdata` code targets | 42,534 |
| aligned `.data` pointers into `.text` | 7,479 |
| unique `.data` code targets | 1,458 |

A vtable is counted only when an aligned `.rdata` pointer resolves to a validated MSVC x86 CompleteObjectLocator and the immediately following entries contain one or more VAs inside `.text`. This avoids treating every long code-pointer table as a C++ vtable.

The RTTI population is heavily dominated by Havok (`hk*` / `hkp*`) classes, which is expected for this build and provides useful high-confidence class/method anchors. Non-RTTI game systems must additionally be recovered from reflection tables, strings, globals, call graphs, and custom descriptors; therefore this phase does **not** claim that 494 is the total class count.

## Function-boundary impact

Virtual method targets are independent evidence for Phase 11 candidates. They are used as confidence evidence, not blindly injected as new function boundaries unless the target decodes as a valid instruction inside `.text`.

## Reproducibility

Run:

```bash
python tools/inventory/rtti_vtable_inventory.py /path/to/RE5DX9.exe
```

The scanner writes a deterministic summary and a detailed derived `reports/vtables.json`. The executable itself must never be committed.

## Gate rule

Phase 12 may close only after the derived summary is committed, the scanner is reproducible on the exact target hash, CI is green, and no RTTI/vtable count is represented as decompilation progress.
