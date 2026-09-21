# Ordered decompilation evidence — 0x0041CDE0 through 0x0041D230

## FUN_0041CDE0
- Factory allocating `0x4C` bytes aligned 0x10 through global allocator; calls base `0x00D02A20`, clears state bytes `+0x04..+0x07`, installs vtable `0x0137E8C0`, returns object/null.

## FUN_0041CE20
- UI/input handler active only when root flag low byte has bit 2.
- State byte `+0x05`: initializes two selection columns (`+0x28`, values at `+0x2C/+0x30`, maxima `+0x3C/+0x40`) or processes input mask from global input object `0x016E2FD0+0x198`.
- Input masks `0x40040/0x10010` change active column; `0x20020/0x80080` increment/decrement selected value with bounds.
- Special input condition stores selections to root `+0x1E5BC/+0x1E5C0` and calls current-user wrapper `0x0041D1D0(1)`.
- Calls debug/UI renderer `FUN_0041C420` after changes; returns bool indicating handled/change.

## FUN_0041CF40
- Three-state asynchronous loader/initializer returning bool.
- State 0 waits service `0x0050B3F0`, resolves token `0x0137E9A8` and optional `0x004070C0`, calls `0x0050B350`, advances.
- State 1 waits `0x005092D0`, resets global timer, calls `0x004897D0(0)` and `0x0050B370(0)`, advances.
- State 2 waits `0x005092D0` false then calls huge resource bootstrap `FUN_0041C5E0` and returns false; other active states return true.

## FUN_0041D000
- Top-level four-state coordinator for this UI/bootstrap object.
- State 0 sets state 1, calls global vtable/service, resolves token `0x0137E9F0`, writes result to service byte `0x016E1D3C+0x904D`.
- State 1 runs `FUN_0041CF40`; on completion enables `0x00CF6710`, resets global UI vtable, sets root byte `+0x1E4EB=1`, advances to state 2 and invokes `FUN_0041CE20`.
- State 2 waits until input handler returns false then state 3; resolves token `0x0137E9D8`, branches on returned object mode and invokes global callbacks / sets global state `+0x24=5`.
- Always updates global object `0x016E1B54` and, under debug root flag `0x08000000`, renders its four state bytes with `0x004BC370`.

## FUN_0041D1D0
- Synchronized current-user wrapper forwarding one arg to `0x00B7CEB0` on current-user structure.

## FUN_0041D220
- Returns constant address `0x01657EC4`.

## FUN_0041D230
- Deleting-destructor wrapper: imported cleanup on `this+0x08`, restores vtable `0x0137A97C`, optionally frees through global allocator vtable `+0x18`, returns self.

Next ordered candidate `0x0041D270`. No VERIFIED/MATCHED claim.