# Ordered decompilation evidence — 0x0040AFC0 through 0x0040B1C0

Target: verified RE5DX9 1.1.0 Dev Release executable used by the project.

## FUN_0040AFC0

- Entry is preceded by compiler INT3 padding at `0x0040AFBD..0x0040AFBF`.
- One stack argument; `ret 0x04` on all exits.
- Null input immediately returns false.
- For non-null input the object vtable method at slot `+0x10` is invoked repeatedly.
- Returned descriptor fields used by this routine include byte `+0x23` and DWORD `+0x20`; bits 8..23 of the latter are extracted.
- Byte `+0x23` selects a seven-way jump table at `0x0040B0CC` after subtracting one. Several cases accept fixed IDs/ranges, while other cases reject.
- A special accepted branch consults global root `0x016E1660`, follows `+0x1E4EC`, and accepts only when DWORD `+0x58 == 1`.
- Accepted identifiers visible in the decoded CFG include `0x14`, `0x18`, `0x20`, `0x26`, `0x924`, `0x120A..0x120D`, `0x1230..0x1231`, selected `0xA001..0xA00F` cases through the byte table at `0x0040B0F0`, `0xD002`, `0xDC00`, `0xF008`, the `0x01xx` class, and `0x0506` depending on the descriptor category.
- This is therefore a descriptor/event eligibility predicate. Semantic naming remains provisional until the descriptor type is recovered.

## FUN_0040B100

- Entry `0x0040B100`; ends at `0x0040B157`, followed by INT3 padding.
- `stdcall`, two stack arguments, `ret 0x08`.
- Null first argument returns `0xFFFFFFFF`.
- Invokes first object's vtable slot `+0x10` and reads returned descriptor DWORD `+0x20`.
- If that DWORD equals argument 2, calls `0x004E5F00` on global object `0x0165BBB0` with value `0x100` and returns its result.
- Otherwise accepts only values whose high byte is `0x07`; extracts bits 8..23, passes that 16-bit value to the same `0x004E5F00`, and returns its result.
- All other paths return `0xFFFFFFFF`.
- Conservative interpretation: maps a descriptor/event code to an index/value through the global service at `0x0165BBB0`.

## FUN_0040B160

- Entry follows compiler INT3 padding; `thiscall`, one stack argument, `ret 0x04`.
- Preserves `this` in EDI and the stack object in ESI.
- Calls `0x00523330(global 0x0165C260, object)` as a gate; false exits without mutation.
- Rejects object state DWORD `+0x2ED8 == 2`.
- Reads object pointer `+0x131C`; uses it only when non-null and byte `+0x48 != 0`, otherwise passes null.
- Calls `0x004E8860` on global `0x0165BBB0` with the selected pointer and literal `1`.
- Uses object byte `+0x2ECC` as an index and stores the returned value into `this + 0x498 + index*4`.
- This is a gated per-slot handle/resource refresh using the object's current slot index.

## FUN_0040B1C0

- Entry follows compiler INT3 padding; `thiscall`, two stack arguments, `ret 0x08`.
- Same `0x00523330(global 0x0165C260, object)` gate and `object+0x2ED8 != 2` requirement as `FUN_0040B160`.
- Uses stack argument 2 directly, calls `0x004E84F0(global 0x0165BBB0, arg2, 1)`.
- Stores the returned value at `this + 0x498 + object[0x2ECC]*4`.
- This is the sibling path to `FUN_0040B160`, differing only in how the value supplied to the global service is obtained.

## Boundary continuation

After `FUN_0040B1C0`, the next aligned post-INT3 candidate is `0x0040B210`. Additional aligned candidates observed in the immediate ordered region are `0x0040B3D0`, `0x0040B440`, `0x0040B4A0`, `0x0040B4E0`, `0x0040B560`, `0x0040B920`, `0x0040B970`, `0x0040B9E0`, `0x0040BAA0`, `0x0040BB30`, `0x0040BBC0`, `0x0040BD30`, `0x0040BDB0`, `0x0040BE40`, `0x0040BF20`, and `0x0040BFD0`.

No VERIFIED/MATCHED status is claimed by this evidence note.