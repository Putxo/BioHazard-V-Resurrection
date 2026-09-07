# BioHazard V Resurrection

From-scratch Resident Evil 5 PC 1.1.0 Dev decompilation/reimplementation project, organized around a canonical binary inventory and real C/C++ source recovery.

> **Foundation and Inventory are validated. Full-executable V5 fixed-point CFG re-audit baseline `RE5DX9_DEV_2017_INVENTORY_V5_FIXED_POINT_CFG_REAUDIT` uses 79,016 high-confidence machine-code entrypoints; PILOT remains single-coordinator only.**

## Progress

<!-- progress:start -->
```
Functions   ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░    0.0%  3 / 79,016
Code size   ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░    0.0%  11 / 15,568,343
```
<!-- progress:end -->

The tiers below are independent dimensions. A function can eventually be MATCHED without yet being fully CONVERTED/readable, or CONVERTED before exact matching is available.

<!-- tiers:start -->
```
FAST PASS  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░    0.0%  3 / 79,016
CONVERTED  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░    0.0%  3 / 79,016
REFINED    ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░    0.0%  0 / 79,016
VERIFIED   ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░    0.0%  0 / 79,016
MATCHED    ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░    0.0%  0 / 79,016
LINKED     ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░    0.0%  0 / 79,016
```
<!-- tiers:end -->

The denominator is a **high-confidence machine-code entrypoint working baseline**, not a claim of the exact original C/C++ source-function count. V5 accounts for the complete 19,977,216-byte PE, disassembles all of `.text` with GNU objdump and LLVM llvm-objdump, and then requires fixed-point CFG reachability before linear-decoder references can create new function roots. The matching `BH5DCRelease.pdb`/MAP is unavailable.

## Progress atlas

The atlas is driven from canonical metadata. Inventoried-but-unimplemented code starts gray; recovered functions change color only when their corresponding tier/status evidence exists.

![Decompilation progress treemap](reports/progress-treemap.svg)

[Interactive Progress Atlas](reports/index.html)

### Color meaning

- dark green — MATCHED
- green — VERIFIED
- light green — REFINED
- teal — CONVERTED
- yellow-green — FAST_PASS_VALIDATED
- yellow — FAST_PASS
- orange — DISASSEMBLED / DISCOVERED
- red — BLOCKED
- gray — UNSEEN

## Inventory baseline

- Target SHA-256 — `323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`
- V5 fixed-point high-confidence machine-code entrypoints — **79,016**
- Previous V4 working denominator — 79,493; **477 entries removed from high-confidence**
- Complete raw file bytes accounted — **19,977,216 / 19,977,216**
- Common instruction starts across the complete `.text` pass — **4,835,646**
- GNU-only instruction starts — **912**
- LLVM-only instruction starts — **661**
- Common 16-byte-aligned post-`INT3` roots — **72,919**
- Reachable direct-CALL targets — **16,571**
- Direct/padding/entry base union — **74,589**
- Strong `.rdata` / `.data` address-taken additions — **4,338**
- Strict reachable immediate callback additions — **45**
- Strong reachable tail-entry additions — **44**
- Fixed-point iterations — **2**, with **0 new roots** in the second iteration
- Reachable common instructions — **3,726,072**
- Reachable instruction bytes — **13,772,419 / 16,185,403 `.text` virtual bytes (85.09%)**
- MSVC RTTI TypeDescriptors — **433**
- CompleteObjectLocators — **494**
- Confirmed structural MSVC vtables — **494**
- Vtable slots — **3,534**
- Imports — **376 across 21 DLLs**
- Exports — **0**
- TLS / exception / base-relocation directories — **absent**

See [`config/inventory_freeze.json`](config/inventory_freeze.json), [`reports/function-count-audit.md`](reports/function-count-audit.md), [`reports/full-executable-reaudit-v4.md`](reports/full-executable-reaudit-v4.md), and [`reports/full-executable-reaudit-v5.md`](reports/full-executable-reaudit-v5.md).

## Roadmap state

- Foundation — ✅ PASS
- Binary / Function / XREF / RTTI / Global Inventory — ✅ PASS
- Full Executable Re-audit V5 — ✅ complete full-PE + fixed-point CFG pass
- Working high-confidence entrypoint baseline — **79,016**
- Exact original source-symbol function count — **unavailable without matching PDB/MAP**
- Pilot — 🟡 ACTIVE (single coordinator)
- Multi-agent Scale — 🔒 LOCKED

## What matching means

The exact PC/x86 MATCHED criterion will be refined during Pilot from reproducible function-level evidence. No function is marked MATCHED merely because its address or pseudocode is known.

## Build

```sh
cmake --preset dev
cmake --build --preset dev
ctest --preset dev
```

## Setup

The target executable is supplied locally by the user and remains git-ignored. Original EXEs, DLLs and game assets are never committed.

## Coordination

One coordinator owns Foundation, Inventory and Pilot. Parallel work remains disabled until several real functions complete the full pipeline without ownership or CI failures.

## Legal and scope

This repository contains original source, tooling, metadata and documentation. Proprietary game binaries/assets remain local.
