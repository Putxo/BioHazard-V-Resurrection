# Ordered decompilation evidence — 0x0040BF20 through 0x0040C270

## FUN_0040BF20
- `thiscall`, one integer stack arg, `ret 0x04`.
- Stores argument to global state `+0x684`, sets byte `+0x688 = 1`.
- Adds integer argument (converted to float) to global timer `+0x67C`.
- When old timer is below `0x01569F38` but old timer + increment reaches/exceeds `0x013EF140`, selects random ID `0x3D..0x40` and calls `FUN_0040B4A0`.
- Timer increment plus threshold-crossing feedback.

## FUN_0040BFD0
- `stdcall`, slot index.
- Sets per-slot byte `+0x6A4 = 1`, timer `+0x6A8 = float[0x01569F40]`, DWORD `+0x6AC = 0` using `slot<<6`.
- Starts/resets per-slot timed state consumed by `FUN_0040BB30`.

## FUN_0040C030
- `thiscall`, no args.
- Ensures byte `this+0x04 == 0x0F`; when different, zeroes DWORD `+0x04` then writes byte value `0x0F`.
- Small state/type initializer.

## FUN_0040C050
- `thiscall`, no stack args.
- Sets global byte `+0x798 = 1`.
- Calls `0x00472400` on global `0x016E2F24`, then sets bytes at that object's `+0x1848` and `+0x1828`.
- Reads global byte `+0x799` and dispatches one of IDs `0x323`, `0x324`, or default `0x320` through sibling `0x0040AAE0(this,id)`.
- Initialization/transition dispatch based on a global substate byte.

## FUN_0040C0D0
- `thiscall`, bool return; state machine keyed by byte `this+0x06`, jump table at `0x0040C1E8`.
- Uses global service `0x0165C148`, helpers `0x0050B410`, `0x005092D0`, `0x0050B230`.
- Owns pointer `this+0x134`; replacement path clears flag `0x2000`, invokes virtual release slot `+0x30`, obtains replacement through `0x008A25C0(0x016A1CD8)`, sets flag `0x2000`, and registers via `0x00D05520(global 0x016E1690,0x1B,obj)`.
- Calls vtable slot `+0x34` on global object `0x016922AC` during one transition.
- Most states increment `this+0x06`; terminal path calls `0x0050B230(...,1)` and returns true. Other paths return false.
- Conservative interpretation: staged asynchronous resource/setup state machine.

## FUN_0040C240
- `thiscall`; sets `this+0x504 = 0` and float `this+0x508 = [0x0137E514]`.
- Two-field reset/default initializer.

## FUN_0040C260
- Exact no-op `ret`.

## FUN_0040C270
- `thiscall`; destructor/cleanup body continues to `0x0040C328`.
- Releases owned interface `this+0x130` through its vtable with deleting/release argument `1` and nulls it.
- Frees allocations at `this+0x348`, `+0x34C`, `+0x144` through target aligned/free routine `0x0125145C`, nulling each.
- Rewrites several embedded vtable pointers to `0x01389064`, then changes top-level vtable through `0x0137A990` and `0x0137A97C` around an imported cleanup call via IAT `0x0137119C` on `this+0x08`.
- Destructor/teardown semantics are strong; exact class name remains unresolved.

Next ordered post-padding candidate follows `0x0040C329`. No VERIFIED/MATCHED claim.