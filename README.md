# BioHazard V Resurrection

From-scratch Resident Evil 5 PC 1.1.0 Dev decompilation/reimplementation project, organized around a canonical binary inventory and real C/C++ source recovery.

> **Foundation and Inventory are validated. Function-entry audit baseline `RE5DX9_DEV_2017_INVENTORY_V2_FUNCTION_ENTRY_AUDIT` is frozen at 79,108 machine-code entrypoints; PILOT remains single-coordinator only.**

## Progress

<!-- progress:start -->
```
Functions   ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░    0.0%  3 / 79,108
Code size   ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░    0.0%  11 / 15,568,343
```
<!-- progress:end -->

The tiers below are independent dimensions. A function can eventually be MATCHED without yet being fully CONVERTED/readable, or CONVERTED before exact matching is available.

<!-- tiers:start -->
```
FAST PASS  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░    0.0%  3 / 79,108
CONVERTED  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░    0.0%  3 / 79,108
REFINED    ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░    0.0%  0 / 79,108
VERIFIED   ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░    0.0%  0 / 79,108
MATCHED    ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░    0.0%  0 / 79,108
LINKED     ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░    0.0%  0 / 79,108
```
<!-- tiers:end -->

The denominator is an **audited machine-code entrypoint baseline**. The original matching `BH5DCRelease.pdb` is not available, so this is not described as a symbol-perfect original-source function count.

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
- Audited machine-code function entrypoints — **79,108**
- Compiler padding-boundary starts — **72,565**
- Additional direct-CALL entrypoints — **1,833**
- Function-pointer-table-only additions — **4,378**
- Explicit callback/immediate-address additions — **331**
- Referenced `.data` global candidates — **21,730**
- MSVC RTTI TypeDescriptors — **433**
- Confirmed MSVC vtables — **494**
- Imports — **376 across 21 DLLs**
- Exports — **0**
- ASCII strings — **44,096**
- `.text` regions (64 KiB) — **247**

See [`config/inventory_freeze.json`](config/inventory_freeze.json) and [`reports/function-count-audit.md`](reports/function-count-audit.md).

## Roadmap state

- Foundation — ✅ PASS
- Binary / Function / XREF / RTTI / Global Inventory — ✅ PASS
- Function Count Audit — ✅ 79,108 machine-code entrypoints
- Inventory Freeze — ✅ PASS
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
