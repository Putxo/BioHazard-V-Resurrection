# Ordered decompilation evidence — 0x0041EBE0 through 0x0041EEF0

- `0x0041EBE0`: allocates 0x14 bytes via `0x01199490`, installs vtable `0x0137EC2C`, returns object/null.
- `0x0041EC00`: no-op.
- `0x0041EC10`: initializes 16 owner records starting `+0x370` with sentinel `-1` in fields 0/+4/+0x0C, zero +8; resets count `+0x368`.
- `0x0041EC50`: appends triple `(arg1,arg2,arg3)` into first free 0x14-byte owner record (`+0x374/+0x378/+0x37C`), increments count; if all 16 occupied logs literal `0x0137EEAC`.
- `0x0041ECB0`: maps caller object/index through global handle service to an external identifier (`object+0xE4->+0x10`), searches owner records by identifier field `+0x374`, returns associated field `+0x378` or zero.
- `0x0041ED50`: same search but returns field `+0x37C`, with `-1` sentinel on failure.
- `0x0041EDF0`: three-output lookup: initializes outputs `(0,-1)`, resolves external identifier, scans records, writes both associated fields `+0x378/+0x37C` to caller outputs on match.
- `0x0041EEA0`: bool setter for owner byte `+0x878`; on change and when global `+0x574==+0x578`, dispatches `0x004A0100(global,0x1C,bool)`.
- `0x0041EEF0`: lazy-creates owner object `+0x560` using allocator and constructor `0x008CBF20`; continuation/configuration follows next.

No VERIFIED/MATCHED claim.