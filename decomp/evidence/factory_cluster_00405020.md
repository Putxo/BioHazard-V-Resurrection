# Ordered factory cluster — 0x00405020 region

Target SHA-256: `323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`

The sequential pass after `FUN_00404F90` reaches a repeated family of allocation/constructor bodies. They share the same allocator global and base initialization sequence but install different final vtable tokens. This file records the common machine contract without assigning class names that are not yet proven by RTTI ownership.

## Common constructor/factory pattern

For each factory below:

1. Load allocator/manager object from global `0x01641A0C`.
2. Dispatch its vtable slot `+0x14` with alignment `0x10` and the listed allocation size.
3. If allocation returns null, return `0`.
4. Call common initializer `0x00D02A20` on the allocation.
5. Temporarily install vtable token `0x0137AA9C`.
6. Construct/initialize the subobject at `object+0x158` via `0x0119C370`.
7. Zero fields `+0x04`, `+0x48`, byte `+0x198`, and dword `+0x19C`.
8. Install the factory-specific final vtable token and return the allocated object.

The bodies have no input arguments and return the new object in `EAX`.

## Individual bodies

| VA | Size | Allocation | Final vtable token |
|---|---:|---:|---:|
| `0x00405020` | `0x58` | `0x1A0` | `0x0137ABA8` |
| `0x00405080` | `0x58` | `0x1A8` | `0x0137ABD4` |
| `0x004050E0` | `0x58` | `0x1A0` | `0x0137AC00` |
| `0x00405140` | `0x58` | `0x1A0` | `0x0137AC2C` |
| `0x004051A0` | `0x58` | `0x1A0` | `0x0137AC58` |
| `0x00405200` | `0x58` | `0x1A0` | `0x0137AC84` |
| `0x00405280` | `0x58` | `0x1A0` | `0x0137ACB0` |

Each listed body is followed by compiler `INT3` padding to the next aligned start. `0x00405260` is a separate short body between the `0x00405200` and `0x00405280` factories and is intentionally not folded into this family.

## Important difference at 0x00405080

`FUN_00405080` requests `0x1A8` bytes rather than `0x1A0`, but the visible common field initialization remains the same through `+0x19C`. The additional tail bytes are therefore preserved as an allocation-size distinction rather than guessed fields.

## Status

All seven entries are **DISASSEMBLED with high-confidence factory semantics**. Their exact allocation sizes, common initializer calls, zeroed offsets, and final vtable tokens are proven. They are not yet marked `FAST_PASS` because the allocator interface and the concrete RTTI/class ownership of the final vtables have not been bound into recovered C++ types. The next ordered non-family body requiring independent analysis is `0x00405260`.
