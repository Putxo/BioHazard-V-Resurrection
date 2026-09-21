# Ordered decompilation evidence — 0x0041DF30 through 0x0041E270

- `0x0041DF30`: looks up current-user value `+0x273E0` in 6-byte table `0x01568A50`; obtains companion WORD and, for modes 3/4 with `0x00AC1890`, remaps selected IDs (`0x3F->0x40`, `0x42->0x43`, `0x44->0x45`); passes final signed WORD to service `0x0050B870`. Disabled by root `0x80000`.
- `0x0041E000`: same table lookup, writes boolean table flag byte `+0x04` into owner byte `+0x29`; no match writes zero.
- `0x0041E050`: two-step global selector-3 state machine using owner bytes `+0x881/+0x882`; starts `0x004A1E60(3)`, waits `0x004A1970(3)`, finalizes `0x00495640(3)`, increments outer byte and resets substate.
- `0x0041E0B0`: removes/clears four players by calling `0x005237A0(index)` for 0..3.
- `0x0041E0D0`: replaces owned object `this+0x624`; releases old `0x2000`-owned object, allocates 0x60-byte object via allocator `0x01641AA8`, constructor `0x009F5DE0`, sets ownership flag, initializes vector floats `+0x30..+0x3C`, flags `+0x1C=0x08000800`, registers type `0x1D`.
- `0x0041E180`: string/type factory. Compares caller record string against three literals (`0x0137ED50`, `0x0137ECF8`, `0x0137ECD0`); for recognized strings allocates 0xC0 object and constructs it with distinct descriptor/selector pairs via `0x011A2930`; otherwise delegates to base factory `0x00D02A50`.
- `0x0041E270`: begins 16-entry owned-object refresh loop over owner records; for each active record releases/replaces object via allocator/constructor `0x00D96E40`, sets ownership flag and configures it from associated owner data. Continuation next.

No VERIFIED/MATCHED claim.