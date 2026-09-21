# Ordered decompilation evidence — 0x0040DBE0 through 0x0040DFF0

## FUN_0040DBE0
- `thiscall`; executes only when current-user record field `+0x273D4` is nonzero.
- Unless root flag `0x20` is already set, iterates 2 or 4 owner pointers starting at `this+0x108` depending on whether global mode is 1; gated by `0x00AC0F30`.
- Existing objects are released via ownership flag `0x2000` + virtual slot `+0x30`; replacements allocated by `0x008322B0(0x016A0E48)`, assigned index at object `+0x38`, flagged `0x2000`, registered as type `0x1B` through `0x00D05520`.
- Finally synchronizes current-user record and clears field `+0x273D4` to zero.

## FUN_0040DD00
- `thiscall`, two stack args, `ret 0x08`.
- Stores args to `this+0xF8/+0xFC` and synchronously ORs root `+0x1E4F8` with flag `0x100`.

## FUN_0040DD60
- Large synchronized per-player/session statistics/state routine ending at `0x0040DFE8`.
- Determines current and opposite player records from global state block `+0x590` using current-user bit and 0x18-byte per-player stride; conditionally mirrors float/DWORD fields `+0x10/+0x14` between the two records depending on global `+0x574/+0x578` equality.
- Iterates two global slot IDs at offsets `+0x490/+0x494` and matching 0x18-byte state records.
- Validates current user/player through services `0x004706A0`, `0x0052FCC0`, `0x004E4FA0`.
- In global mode 0, converts record float values to bounded integer/statistic values, calls `0x00AC1A90` and `0x00AC1C50`, forwards result through `0x0041B480`, and updates a per-user structure obtained through `0x00B7B7E0`, retaining minima for two values.
- Calls `0x004E5030` each processed slot; entire operation is protected by imported synchronization lock/unlock.

## FUN_0040DFF0
- `thiscall`; calls sibling `0x0040A930(0)`.
- Synchronizes and sets root flag `+0x1E4F8 |= 0x40000`.
- Clears owner byte `this+0x128`, sets global state DWORD `+0x24 = 2`.
- Calls vtable slot `+0x24` on global `0x016E1FD0` with address token `0x01657DFC`.
- Strong semantics: transition/initiation wrapper that raises global ready/request flag and dispatches an external callback.

Next sequential code continues at `0x0040E070` (not necessarily aligned post-INT3; boundary must be checked against inventory before enrollment). No VERIFIED/MATCHED claim.