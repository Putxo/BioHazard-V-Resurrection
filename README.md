# BioHazard V Resurrection

From-scratch Resident Evil 5 PC 1.1.0 Dev decompilation/reimplementation project, organized around a canonical binary inventory and real C/C++ source recovery.

> **Foundation is validated. Binary inventory is the next unlocked phase; no decompilation percentage is claimed until the real target inventory exists.**

## Progress

<!-- progress:start -->
```
Functions   ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░  pending
Code size   ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░  pending
```
<!-- progress:end -->

The tiers below are independent dimensions. A function can eventually be MATCHED without yet being fully CONVERTED/readable, or CONVERTED before exact matching is available.

<!-- tiers:start -->
```
FAST PASS  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░  pending
CONVERTED  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░  pending
REFINED    ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░  pending
VERIFIED   ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░  pending
MATCHED    ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░  pending
LINKED     ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░  pending
```
<!-- tiers:end -->

## Progress atlas

Every inventoried function will be one rectangle, sized by its byte count and colored from canonical status/tier metadata. Until real inventory exists the atlas deliberately shows no fake green progress.

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

## Roadmap state

- Foundation — ✅ PASS
- Binary Inventory — 🔓 NEXT
- Function Inventory — 🔒 LOCKED
- Xref / Global / RTTI / Vtable Inventory — 🔒 LOCKED
- Pilot — 🔒 LOCKED
- Multi-agent Scale — 🔒 LOCKED

See [`docs/gates/GATE_09.md`](docs/gates/GATE_09.md) for the remote Foundation Freeze evidence.

## What matching means

The exact PC/x86 MATCHED criterion is intentionally not claimed yet. It will be defined only after the original toolchain and reproducibility constraints are investigated.

## Build

```sh
cmake --preset dev
cmake --build --preset dev
ctest --preset dev
```

## Setup

The target executable is supplied locally by the user and remains git-ignored. Original EXEs, DLLs and game assets are never committed.

## Coordination

One coordinator owns Foundation, Inventory and Pilot. Parallel work will require canonical claims and task branches; `main` and `integration` are not direct work branches.

## Legal and scope

This repository contains original source, tooling, metadata and documentation. Proprietary game binaries/assets remain local.
