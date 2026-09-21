# Ordered decompilation evidence — 0x0041EF50 through 0x0041F430

- `0x0041EF50`: if owner pointer `+0x594` exists, tail-calls `0x00828B10`; otherwise false.
- `0x0041EF70`: same pointer gate, forwards one arg to `0x00828FE0`; absent returns false.
- `0x0041EF90`: scans caller string up to 256 bytes for ordered marker substrings/literals `0x0137EED0` then `0x0137EED8`, using temporary buffers and comparison helpers; returns true only when both marker patterns are found in order.
- `0x0041F090`: if owner `+0x5F0` and `+0x578` non-null, calls `0x006648E0(+0x578,+0x5F0)` and clears `+0x5F0`.
- `0x0041F0C0`: large transform/orientation calculation. Uses global player object `0x016E1FD0+0x3830`, resolves player by stored indices `+0x5E4/+0x5E8`, obtains a 4x4-like transform via virtual slot `+0x44`, then according to state byte `+0x5EC` either copies owner basis vectors `+0x5C0..+0x5D8` or derives normalized transformed basis using matrix multiplication and scale normalization. Writes result vectors into caller output object; state byte `+0x5ED` is advanced/used as one-shot state.
- Body continues to aligned boundary `0x0041F430`.

No VERIFIED/MATCHED claim.