# Ordered decompilation evidence — 0x0040CDE0 through 0x0040D0C0

## FUN_0040CDE0
- Calls `0x00409A40`, requests/looks up a resource through global `0x0165BBB0` using literal/address `0x0137B6B4`, then feeds result and token `0x01692E3C` to object/global `0x016E1D3C` via `0x00D14C10`.
- Passes returned object to global `0x01659B64 -> 0x00469CF0`.
- If non-null, tail-jumps to `0x00D25060` with object as ECX; otherwise returns.
- Resource acquisition/registration wrapper.

## FUN_0040CE30
- `thiscall`, no stack args.
- Validates a large set of owner pointers: `+0xE8`, `+0xEC`, `+0xF0`, `+0x2C`, `+0x100`, `+0x108`, `+0x10C`, `+0x110`, `+0x114`, `+0x118`, `+0x4F4`, `+0x4F8`, `+0x11C`, `+0xD0`, `+0x134`, `+0x4FC`, `+0x500`.
- For every non-null pointer it reads pointee DWORD `+0x04`, masks low 3 bits, and retains the pointer only when resulting state is `1` or `2`; otherwise clears the owner field.
- Bulk stale-handle/resource-pointer sanitization routine.

## FUN_0040D050
- `thiscall`, three stack args, `ret 0x0C`.
- Sets owner active byte `+0x40 = 1` and stores three arguments to `+0x44/+0x48/+0x4C`.
- Under the same root synchronization protocol used by neighboring routines, ORs global root DWORD `+0x1E500` with flag `0x00400000`.
- Request/command setup followed by synchronized global notification flag.

## FUN_0040D0C0
- `thiscall`, no stack args.
- Clears owner active byte `+0x40`.
- Unless owner byte `+0x04` already equals 3, synchronizes on global root, sets root flag `0x2000`, then zeroes owner DWORD `+0x04` and writes byte state `3`.
- Local state reset/transition with synchronized global flag update.

Next ordered entry: `0x0040D130`. No VERIFIED/MATCHED claim.