# Pilot evidence — BB9070 accessor and BB90C0 membership cluster

Target SHA-256: `323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`

## FUN_00BB9070

- VA `0x00BB9070`, RVA `0x007B9070`, `.text`, estimated/original body size 3 bytes.
- Boundary evidence: 6 bytes of preceding `INT3` padding.
- Direct decoded callers: 105.
- Exact body: `8B 01 C3` → `mov eax,[ecx] ; ret`.
- Provisional ABI: MSVC x86 `thiscall`-style; `ECX=this`, return bits in `EAX`.
- C++ representation returns the 32-bit value at offset `+0x00`; no semantic class/member name is inferred.

## FUN_00BB9080

- VA `0x00BB9080`, RVA `0x007B9080`, `.text`, body size 4 bytes.
- Boundary evidence: 13 bytes of preceding `INT3` padding.
- Direct decoded callers: 20.
- Exact body: `8B 41 04 C3` → `mov eax,[ecx+4] ; ret`.
- The BB90C0 caller establishes that this value is a link pointer. The portable C++ representation preserves the observed `+0x04` member offset.

## FUN_00BB9090

- VA `0x00BB9090`, RVA `0x007B9090`, `.text`, body size 4 bytes.
- Boundary evidence: 12 bytes of preceding `INT3` padding.
- Direct decoded callers: 7.
- Exact body: `8B 41 04 C3` → `mov eax,[ecx+4] ; ret`.
- Kept as a distinct stable function despite having the same body as `FUN_00BB9080` because it has a distinct VA and caller set.

## FUN_00BB90C0

- VA `0x00BB90C0`, RVA `0x007B90C0`, `.text`, exact body size 51 bytes (`0x33`).
- Boundary evidence: 15 bytes of preceding `INT3` padding; the body ends at `0x00BB90F2` and is followed by `INT3` padding.
- Direct decoded callers: 5.
- Provisional ABI: MSVC x86 `thiscall`-style. `ECX=self`; stack arguments are `bool* result` and `UnknownBB90Object* candidate`; `ret 8` pops both arguments.
- Exact behavior: starting at `candidate`, compare each node with `self`; if unequal, follow the link returned by `FUN_00BB9080` from offset `+0x04`; write `1` when found or `0` at end-of-chain.
- Both return paths move the first stack argument into `EAX`, write the byte result through it and return that same `bool*`.
- No cycle detection or null check for the output pointer is added because neither exists in the machine body.

## Caller evidence

`FUN_00BB9070` callers:
`00B949BD 00B94A9A 00BAE4DE 00BAE4F3 00BC0BE8 00BC0C59 00BC1EBD 00BC1FAC 00BC24F6 00BC3B9A 00BC40C7 00BC40D4 00BC40EC 00BC40F5 00BC4111 00BC41CB 00BC4212 00BC47D5 00BC4F22 00BC53EE 00BC5D6C 00BC5E1E 00BC5F35 00BC5F3D 00BC60CA 00BC612C 00BC6157 00BC650C 00BC653C 00BC6559 00BC6639 00BC66F4 00BC670E 00BC672B 00BC6887 00BC69BE 00BC6A6D 00BC6A75 00BC707E 00BC7195 00BC71EF 00BC721B 00BC731C 00BC7339 00BC73C4 00BC7C48 00BC7C71 00BC7C90 00BC839F 00BC87B7 00BC8AB3 00BC8C02 00BC8C2F 00BC8DA1 00BC98D5 00BC9947 00BC9CA6 00BC9F71 00BCAC2A 00BCAF07 00BCB2CB 00BCB30E 00BCB374 00BCB431 00BCBCE9 00BCC0AC 00BCC0B5 00BCC0DB 00BCC135 00BCC13E 00BCDA06 00BCF5E3 00BDDD68 00BDDDC6 00CA71AB 00CA739E 01271F94 0127AB35 0127AF6A 0127B3F0 0127B801 0127BE61 0127CB0A 0127D631 0127D64D 0127D83B 0127D886 0127D95B 0127D96F 0127D988 0127D9DD 0127DB93 0127DBAF 0127DBC5 0127E51A 0127E539 0127E5EB 0127E5FA 0127E712 0127E73F 0127E8D4 0127FFFD 01280013 01280423 01284165`

`FUN_00BB9080` callers:
`00BB90D6 00BB9360 00BB9372 00BB96BE 00BB97C2 00BC2411 00BC3C39 00BC3D8F 00BC4223 00BC4235 00BC61A6 00BC61B1 00BC64FA 00BC6505 00BC8AC5 00BCBC8A 00BCBCC3 00BCBE1F 00BCC826 0127E732`

`FUN_00BB9090` callers:
`01271E25 01271E30 01271EB3 01271EC9 01271ED2 01271F3F 01271F4A`

`FUN_00BB90C0` callers:
`00CA6F08 00CA6F8E 00CA701A 00CA70A3 00CAE04C`

All five callers pass a stack-local byte address as the first argument and immediately test `byte ptr [eax]` after the call, confirming both the output-pointer return and byte-sized Boolean result. The second argument is a candidate node pointer; `ECX` is a fixed or computed chain member to search for.

## Validation scope

The unit test validates the reconstructed C++ accessor behavior, link traversal, positive membership, self-membership, negative membership, null-chain behavior and returned output pointer. CI compilation/CTest is required before `FUN_00BB90C0` promotion to `FAST_PASS_VALIDATED`. No `REFINED`, `VERIFIED`, `MATCHED`, or semantic class-name claim is made by this evidence.
