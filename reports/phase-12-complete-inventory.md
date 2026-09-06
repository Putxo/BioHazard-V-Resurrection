# Phase 12 — Remaining Inventory Coverage

Exact target SHA-256: `323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`

This closes the inventory categories required before the Inventory Freeze without storing the proprietary executable or the ~253 MB local disassembly dump.

| Inventory source | Derived result |
|---|---:|
| PE exports | 0 |
| `.data` addresses referenced by decoded `.text` | 21,730 |
| code → `.data` XREFs | 140,106 |
| `.rdata` addresses referenced by decoded `.text` | 42,473 |
| code → `.rdata` XREFs | 135,750 |
| printable ASCII strings in `.rdata` / `.data` | 44,096 |
| strings matching initial gameplay/system keyword families | 5,225 |
| `.text` regions at 64 KiB granularity | 247 |

The 21,730 `.data` addresses are **global candidates**, not 21,730 proven typed globals. Their identities remain address-stable and can be refined as types, sizes, aliases and ownership become known.

The string filter is discovery metadata only. It is intentionally broad and does not assign subsystem semantics by itself.

Reproduction uses the exact local EXE plus a local `objdump -d -Mintel` output through `tools/inventory/remaining_inventory.py`. The objdump output is prohibited from Git because it is a huge derivative dump.
