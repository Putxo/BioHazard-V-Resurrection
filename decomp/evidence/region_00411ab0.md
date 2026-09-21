# Ordered decompilation evidence — 0x00411AB0 through 0x004123F0

## FUN_00411AB0
- Very large `thiscall` debug/status rendering routine, stack aligned to 64 bytes; gated by root debug flag `+0x1E560 & 0x08000000`.
- Uses text renderer/formatter `0x004BC370` and later `0x004BC510`, global UI/context `0x016E201C`, font/context `0x01652E48`, and many literal strings at `0x0137E5xx/0x0137E6xx`.
- Prints owner state bytes `+0x04..+0x07`, current-user/session fields, global mode/timer fields, optional statistic `+0x5D8`, and numerous owner numeric fields (`+0x370,+0x37C,+0x388,+0x39C,+0x3A8,+0x3B4`, etc.) with screen-coordinate descriptors.
- Chooses different status blocks according to current-user field `+0x273C8` and global mode; uses fixed screen origin/line spacing around `0x130` and 8-pixel-like increments.
- Compares caller string against a five-byte literal at `0x0137E5C0`; for matching command/context invokes per-user helper `0x00B67FF0`, and in global modes `3/4` emits additional status based on current-user `+0x273D0`.
- Strong semantics: developer/debug HUD textual diagnostics, not gameplay state mutation except helper-side effects.

## Boundary note
- This debug routine spans multiple apparent data/jump-table regions and reaches the next enrolled post-padding candidate at approximately `0x004123F0`; all intermediate embedded tables are treated as function-local data unless separately proven as callable entries.
- Next ordered candidate: `0x004123F0`.

No VERIFIED/MATCHED claim.