# Ordered decompilation evidence — 0x0041FEE0 through 0x004203B0

- `0x0041FEE0`: if global byte `0x016E157C` clear, applies three owner resource handles `+0x858/+0x85C/+0x860` to root via `0x0048A030`, resets root state `+0x1E8C8=0`, tail-calls `0x00CF5170(global 0x016E1578)`.
- entry `0x0041FF50`: very large constructor for subsystem object (vtable `0x0137EFA8`). Calls base `0x00D02A20`, constructs embedded objects `+0x290/+0x2BC`, installs 16 small vtable objects across `+0x36C`, constructs `+0x4B0`, initializes multiple embedded dynamic containers (`+0x580,+0x598,+0x5F4`), sets 32 handle slots `+0x2E8..+0x364` to `-1`, clears two 16-entry object arrays and broad state fields, masks root flags, constructs `+0x60C`, zeroes many state bytes/timers/buffers, returns self.
- `0x00420290`: destructor counterpart. Tears down `+0x60C`, clears/deletes embedded containers `+0x5F4/+0x598/+0x580`, destroys `+0x4B0`, restores/destroys 16 small vtable records, destroys `+0x2BC/+0x290`, performs imported cleanup on `this+0x08`, restores base vtable.
- `0x004203B0`: delayed reset routine. Increments owner byte `+0x882`; once >=4, repeatedly acquires root lock and zeros root flag DWORD groups beginning `+0x1E500/+0x1E504/...`; body continues through next boundary `0x004206D0`.

No VERIFIED/MATCHED claim.