# Ordered decompilation evidence — 0x0041ACA0 through 0x0041B100

## FUN_0041ACA0
- Enables player manager and dispatches owner byte `+0x05` states 0..4 to `FUN_00419760`, `0x004125D0`, `0x004126F0`, `0x004127F0`, `FUN_00417B30`; each path then calls `FUN_0040F020(false)`.

## FUN_0041AD30
- Five-state dispatcher: `0x00412D10`, `0x00413280`, `0x00413490`, `FUN_00419AC0`, `0x00413890`; each then tail-calls central updater `FUN_0040E930`.

## FUN_0041ADA0
- Six-state dispatcher: `0x0040E670`, `0x0040A450`, `0x0040A510`, `0x0040E700`, `0x0040E860`, `FUN_0041AAA0`; after each calls `FUN_0040F500` and `FUN_0040F020(false)`.

## FUN_0041AE60
- High-level main update dispatcher.
- First checks global/UI predicate `0x004BD620`, sanitizes owner resource pointers via `FUN_0040CE30`, rejects root flags `0x500` and global root busy state.
- Dispatches owner state byte `+0x04` values 0..15 to recovered state-machine families: `FUN_0041AC80`, `FUN_0041ACA0`, `FUN_00419990`, `FUN_00417B70`, `FUN_0041AD30`, `FUN_00417E50`, `0x00414510`, `FUN_00418460`, `FUN_004184C0`, `FUN_00419FA0`, `FUN_0041A8C0`, `0x00415720`, `0x004158C0`, `FUN_0041ADA0`, `0x004159B0`, `0x00415BE0`.
- Afterwards always runs `FUN_0040D880`, `FUN_00414A40`, `0x00409F00`; copies four owner bytes from buffer `+0x348` to mirror buffer `+0x34C`; renders debug HUD `FUN_00411AB0(0x0137E5C0)`; tail-updates global services `0x004E60B0/0x004E6390/0x004E64B0`.
- Strong semantics: primary top-level update/tick for this recovered subsystem.

## FUN_0041B000
- `thiscall`, one source pointer, `ret 0x04`; copies a compact ~0x2C-byte record field-by-field (DWORD, bytes/WORD, six float fields) into `this`; returns destination.

## FUN_0041B060
- Three-integer helper; returns median of its three integer arguments (branch-based min/max selection).

Next ordered candidate `0x0041B0A0`. No VERIFIED/MATCHED claim.