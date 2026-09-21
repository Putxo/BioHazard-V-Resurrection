# Ordered decompilation evidence — 0x0041A8C0 through 0x0041ACA0

## FUN_0041A8C0
- `thiscall`; state machine on owner byte `+0x05` up to 0x0B.
- Requests root `0x800`, flag groups `0xFFE02000/0x8000`, root flags `1/0x200`, resets timer/UI, seeds owner timer; waits UI readiness and `FUN_00412BF0`.
- Clears root flags `0x4000`, `0x08000000`, `1`; requests `0x40000`, calls `FUN_00418790(0)`, conditionally re-requests/clears `0x40000`, updates global state `+0x24=2`, invokes global callback token `0x01657DFC`.

## FUN_0041AAA0
- `thiscall`; four-state submachine on owner byte `+0x06`.
- Waits global `+0x131C` clear, calls `0x004A6950`, resets global user services `0x004E9920/0x004E9990`, starts UI/timer; waits UI readiness and `FUN_00412BF0`.
- Final state clears root flags `0x4000`, `0x08000000`, `1`, `0x10`, requests `0x40000`, calls `FUN_00418790(0)`, forwards owner `+0x350/+0x354` through `0x0040A580` and `0x004E7AF0`, resets services and global state `+0x24=2`, invokes callback token.

## FUN_0041AC10 (entry after embedded jump table)
- `thiscall`, four stack args, `ret 0x10`.
- Builds compact event record from object via `0x0040F7C0`, `0x004E84F0`, object byte `+0x2011`, caller slot mask and byte, then forwards record plus caller arg to `FUN_004192E0`.

## FUN_0041AC80
- Two-state dispatcher on owner byte `+0x05`: state 0 tail-jumps `FUN_004196A0`, state 1 tail-jumps `0x00409D40`.

## FUN_0041ACA0
- Next dispatcher begins; continuation next.

No VERIFIED/MATCHED claim.