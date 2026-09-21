# Ordered decompilation evidence — 0x0041E3F0 through 0x0041E670

- `0x0041E3F0`: releases/nulls two arrays of 16 owned objects at owner `+0x248` and `+0x208`, plus single owned object `+0x288`, using ownership flag `0x2000` and virtual slot `+0x30`.
- `0x0041E480`: `(groupIndex,bool)`; for group <16 iterates child objects of owner `+0x248[group]` via `0x00D9A9C0/0x00D9A880`, sets/clears child flag `0x800`.
- entry `0x0041E4F0`: sibling identical traversal toggling child flag `0x400`.
- entry `0x0041E560`: `(groupIndex,bitIndex,enable)`; iterates children and sets/clears `1<<bitIndex` in child byte `+0x07`.
- `0x0041E5E0`: readiness predicate for owner object `+0x288`; true when absent or its state `+0x3C==0`; otherwise compares truncated float `+0x1C` against `0x00D97FA0(obj)-2`.
- `0x0041E640`: `(arg)` predicate for object `+0x288`; calls `0x00D97B60(arg)` and returns whether resulting byte differs from object byte `+0x28`.
- `0x0041E670`: begins command/parser routine. Strips prefix/delimiter from caller string, copies remainder to local buffer, compares against three command literals with numeric selectors 1/0/2, and dispatches corresponding global/UI operations. Body continues to boundary `0x0041E870`.

No VERIFIED/MATCHED claim.