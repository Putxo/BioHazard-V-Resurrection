# BioHazard V Resurrection

From-scratch Resident Evil 5 PC 1.1.0 Dev decompilation/reimplementation project, organized around a canonical binary inventory and real C/C++ source recovery.

> **Foundation and Inventory are validated. Inventory baseline `RE5DX9_DEV_2017_INVENTORY_V1` is frozen; PILOT is the next phase.**

## Progress

<!-- progress:start -->
```
Functions   ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░    0.0%  0 / 71,993
Code size   ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░    0.0%  0 / 15,568,343
```
<!-- progress:end -->

The tiers below are independent dimensions. A function can eventually be MATCHED without yet being fully CONVERTED/readable, or CONVERTED before exact matching is available.

<!-- tiers:start -->
```
FAST PASS  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░    0.0%  0 / 71,993
CONVERTED  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░    0.0%  0 / 71,993
REFINED    ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░    0.0%  0 / 71,993
VERIFIED   ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░    0.0%  0 / 71,993
MATCHED    ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░    0.0%  0 / 71,993
LINKED     ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░    0.0%  0 / 71,993
```
<!-- tiers:end -->

The denominator is a **versioned candidate-function baseline**, not a claim that debug symbols prove exactly 71,993 source functions. Boundary corrections require evidence and an explicit baseline revision.

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
- Function candidates — **71,993**
- Referenced `.data` global candidates — **21,730**
- MSVC RTTI TypeDescriptors — **433**
- Confirmed MSVC vtables — **494**
- Imports — **376 across 21 DLLs**
- Exports — **0**
- ASCII strings — **44,096**
- `.text` regions (64 KiB) — **247**

See [`config/inventory_freeze.json`](config/inventory_freeze.json) for the frozen machine-readable baseline.

## Roadmap state

- Foundation — ✅ PASS
- Binary / Function / XREF / RTTI / Global Inventory — ✅ PASS
- Inventory Freeze — ✅ PASS
- Pilot — 🔓 NEXT
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
