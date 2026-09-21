# Ordered decompilation evidence — 0x00416BC0 through 0x00417120

## FUN_00416BC0
- `thiscall`, three stack args, `ret 0x0C`; only when global `+0x574==+0x578`.
- One-shot guards source event byte `+0x19CC`, derives two classification bytes through `0x0040F7C0` and `0x004E84F0`, includes source byte `+0x2011`, caller slot mask and caller byte.
- In global mode 4, type token `0x016966A8` sets bit `0x80` in compact record.
- Dispatches compact record through `0x004A08B0(global 0x016E03CC)`.

## FUN_00416CA0 / FUN_00416D20 / FUN_00416DA0
- Three sibling player wrappers using per-slot one-shot byte owner `+0x4A8`.
- Validate player and reject state 2, set per-slot byte, call `FUN_00411640(player, value/0)`.
- For state 0, derive mapped value either from `0x004E8860(caller)` (`416CA0`), owner `+0x498` when <=99 (`416D20`), or `0x004E84F0(caller,1)` (`416DA0`), then call `FUN_004106C0(1,slot,mapped)`.

## FUN_00416E20
- `thiscall`, one value arg, `ret 0x04`.
- Iterates first two global handles `+0x490/+0x494`; when global byte `+0x798` clear and both current/player handles validate, locks owner, opens each global handle, calls `0x0041B900(value)`, finalizes handle.
- Broadcast value to two eligible handles.

## FUN_00416F00
- `thiscall`; rank/score-to-owner-fields updater under `0x00AC1210` gate.
- Reads four global scores `+0x6B4/+0x6F4/+0x734/+0x774`; depending on `0x00AC11D0`, either derives winning group from comparisons and global grouping `+0x4C0..+0x4CC`, or finds maximum among four scores.
- Uses constants bytes `0x01569E11/12` as base/winner values.
- For active bits in global mask `+0x478`, writes/clamps corresponding owner fields (`+0x370`, `+0x39C`, `+0x3C8`, and fourth analogous field) against their maxima.
- Next aligned candidate `0x00417120`.

No VERIFIED/MATCHED claim.