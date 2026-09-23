# Candidate rejection evidence — 0x00407BE0

## Conclusion

`0x00407BE0` is **not a valid function entry** in the exact Resident Evil 5 PC 1.1.0 Dev target.

Unlike the nearby false positives that landed inside instruction bytes, `0x00407BE0` is a genuine instruction boundary. However, exact control flow proves it is the aligned loop basic-block target **inside** the real function rooted at `0x00407BC0`, not a separately callable function.

Creating a C++ implementation, unit test, `database/functions.csv` row, claim, FAST_PASS, or FAST_PASS_VALIDATED record for `FUN_00407BE0` would therefore split one native function into two and fabricate a source-level entry that does not exist.

## Exact target

- executable: `9) RE5DX9_1.1.0 Dev (Release 15 des. 2017)(7).exe`
- executable SHA-256: `323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`
- size: 19,977,216 bytes
- image base: `0x00400000`
- candidate VA: `0x00407BE0`
- candidate RVA: `0x00007BE0`

The locally available reference executable was rehashed before this classification and exactly matches the canonical target SHA-256 above.

## Instruction and control-flow evidence

GNU `objdump` and LLVM `llvm-objdump` independently agree on the native stream:

```text
00407BC0  mov  eax,[ecx]
00407BC2  mov  edx,[eax+10h]
00407BC5  call edx
00407BC7  mov  ecx,[esp+04h]
00407BCB  mov  ecx,[ecx+04h]
00407BCE  cmp  [eax+04h],ecx
00407BD1  jne  00407BD8h
00407BD3  mov  al,01h
00407BD5  ret  4
00407BD8  mov  eax,[eax+10h]
00407BDB  test eax,eax
00407BDD  je   00407BECh
00407BDF  nop
00407BE0  cmp  [eax+04h],ecx
00407BE3  je   00407BD3h
00407BE5  mov  eax,[eax+10h]
00407BE8  test eax,eax
00407BEA  jne  00407BE0h
00407BEC  xor  al,al
00407BEE  ret  4
00407BF1  int3
...
00407BFF  int3
00407C00  push esi
```

Exact bytes for the real `0x00407BC0` function body (`0x31` / 49 bytes):

```text
8B 01 8B 50 10 FF D2 8B 4C 24 04 8B 49 04 39 48 04 75 05
B0 01 C2 04 00 8B 40 10 85 C0 74 0D 90 39 48 04 74 EE 8B
40 10 85 C0 75 F4 32 C0 C2 04 00
```

Body SHA-256:

```text
55c18b3e5aea58ce310701d9ffeb7fced3d762fa75f235928e803675815993d2
```

The candidate instruction at `0x00407BE0` is `39 48 04` (`cmp dword ptr [eax+4], ecx`). Its semantics and incoming edge are internal to the traversal loop:

- the initial node comparison occurs at `0x00407BCE`;
- if it does not match, the function advances to the next link at `0x00407BD8`;
- `0x00407BE0` is the repeated comparison for subsequent links;
- `0x00407BEA` branches back to `0x00407BE0` while another link remains;
- both success (`0x00407BD5`) and failure (`0x00407BEE`) return through the same function rooted at `0x00407BC0`.

Thus `0x00407BE0` is an **internal loop header/basic-block entry**, not a function boundary.

## Xref evidence

A complete disassembly scan of direct relative CALL/JMP/Jcc targets in the exact `.text` stream finds:

- many direct CALL references to the real entry `0x00407BC0` (for example `0x00402BCD -> call 0x00407BC0`);
- exactly one decoded direct control-flow reference to `0x00407BE0`: `0x00407BEA -> jne 0x00407BE0`, the loop back-edge inside the same function;
- no external CALL to `0x00407BE0`.

A whole-file raw little-endian pointer scan finds **zero occurrences** of pointer value `0x00407BE0`.

The next aligned function at `0x00407C00` independently has many direct CALL/JMP references, reinforcing the surrounding real boundaries.

## Inventory-policy consequence

The raw V6 union remains **79,782** potential starts for audit. Current integration already contains five evidence-backed removals at `0x00407A10`, `0x00407AB0`, `0x00407AD0`, `0x00407BA0`, and `0x00407B60`.

This sixth evidence-backed correction reduces the canonical dashboard denominator from **79,777** to **79,776** while leaving recovered-function count, recovered-byte count, and FAST PASS count unchanged.

## Scope

- No C++ implementation is created for `FUN_00407BE0`.
- No unit test, `database/functions.csv` row, claim, FAST_PASS, or FAST_PASS_VALIDATED state is created.
- Existing `FUN_00407B10`, `FUN_00407AF0`, and the published `0x00407B60` / `0x00407BA0` corrections are preserved and not modified.
- No EXE, DLL, proprietary asset, `.obj`, or binary product is committed.
