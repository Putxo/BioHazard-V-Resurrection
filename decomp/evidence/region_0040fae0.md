# Ordered decompilation evidence — 0x0040FAE0 through 0x0040FC80

## FUN_0040FAE0
- `thiscall`, two stack args, `ret 0x08`; runs only in global mode 0.
- Under owner lock iterates two global handles `+0x490/+0x494`, skipping current player handle `0x016E2FD0+0x630` unless valid.
- Validates handles through `0x0052FCC0`; for each eligible handle calls `0x004E4FA0`, invokes external routine `0x0087DF30` with caller-supplied argument, then finalizes `0x004E5030`.
- Two-player/handle broadcast wrapper.

## FUN_0040FBD0
- `thiscall`, no stack args; under owner lock iterates same two global handles.
- Validates each through `0x0052FCC0`, calls `0x004E4FA0`, then forwards owner ID `this+0x64` through `0x004E98A0`, finalizing with `0x004E5030`.
- Broadcast current owner ID to eligible player handles.

## FUN_0040FC80
- Very large `thiscall` routine; excluded for global modes `3/4`.
- Captures current-user fields `+0x273D0/+0x273C8`, current player handle `0x016E2FD0+0x630`, locks owner, and iterates per-player/global handles from `+0x490`.
- Uses `0x00AC1A20` and repeated `0x0052FCC0` validation; for eligible handles calls `0x004E4FA0`.
- Reads per-user state bytes in a `0x3B8`-stride structure and conditionally calls `0x0041B530` / `0x004E8340`.
- Queries per-user collection through `0x00B7AE70` and `0x00B7CCB0`; checks root/per-user flags including `0x40000` and value threshold `0x4650`; may call `0x0041B370(0x12)` and `0x00AC1A50`.
- Performs synchronized mutations of current-user flag DWORD around offset `+0x14C`, setting bits `0x02/0x04`, and invokes `0x00B7A530` with paired selectors.
- Body continues far beyond `0x00410000` to a later boundary; this function remains active.

No VERIFIED/MATCHED claim.