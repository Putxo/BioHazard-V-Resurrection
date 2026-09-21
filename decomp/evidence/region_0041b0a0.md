# Ordered decompilation evidence — 0x0041B0A0 through 0x0041B530

## FUN_0041B0A0
- Exact tail-jump thunk to `0x00409750`.

## FUN_0041B0B0 / FUN_0041B100
- Synchronized root flag setters. With optional lock protocol, OR caller mask into root DWORD `+0x1E4F4` (`B0B0`) or `+0x1E4F8` (`B100`).

## FUN_0041B150 / FUN_0041B1A0
- Synchronized indexed flag group set/clear helpers over root array `+0x1E4FC + index*4`; `B150` ORs mask, `B1A0` ANDs inverse mask.

## FUN_0041B1F0
- Synchronized getter indexed by caller value with stride `0x5E0`; returns DWORD at root `+0x34 + index*0x5E0`.

## FUN_0041B240 / FUN_0041B250
- Exact WORD accessors returning object fields `+0x13F0` and `+0x13F2`.

## FUN_0041B260
- Synchronized current-user setter: writes caller DWORD into current-user record with stride `0x8C74`, field `+0x148`.

## FUN_0041B2B0
- Synchronized current-user bit setter: computes `1<<arg`, ORs it into per-user structure field around `+0x14C` (`base +0x38 +0x114`).

## FUN_0041B310
- Synchronized wrapper forwarding two args to per-user helper `0x00B7A530` on current-user structure.

## FUN_0041B370 / FUN_0041B3D0
- Synchronized current-user bit setters for structure fields `+0x124` and `+0x128` relative to per-user base.

## FUN_0041B430
- Synchronized no-arg wrapper invoking `0x00B7A830` on current-user structure.

## FUN_0041B480
- Synchronized one-arg wrapper invoking `0x00B7B050` on current-user structure.

## FUN_0041B4D0 (entry between aligned candidates)
- Synchronized bit setter on a much later per-user field `+0x4624` relative to structure base.

Next ordered aligned candidate `0x0041B530`. No VERIFIED/MATCHED claim.