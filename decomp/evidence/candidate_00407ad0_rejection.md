# Candidate rejection evidence — 0x00407AD0

## Conclusion

`0x00407AD0` is **not a valid function entry** in the exact Resident Evil 5 PC 1.1.0 Dev target. It is one byte inside the 5-byte relative `call` instruction beginning at `0x00407ACE`.

This candidate must therefore be removed from the V6 potential-function-start universe rather than represented by fabricated C++, tests, a `database/functions.csv` row, or FAST_PASS metadata.

## Exact target

- executable SHA-256: `323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`
- size: 19,977,216 bytes
- image base: `0x00400000`
- candidate VA: `0x00407AD0`
- candidate RVA: `0x00007AD0`

## Instruction-boundary evidence

GNU `objdump` on the exact local reference decodes the surrounding stream as:

```text
00407A90  mov ecx,[esp+04h]
00407A94  sub esp,0Ch
00407A97  test ecx,ecx
00407A99  ja 00407AB1h
...
00407ABD  lea eax,[esp+10h]
00407AC1  push eax
00407AC2  lea ecx,[esp+04h]
00407AC6  mov dword ptr [esp+14h],0
00407ACE  call 01251B3Dh
00407AD3  push 01563F18h
00407AD8  lea ecx,[esp+04h]
00407ADC  push ecx
00407ADD  mov dword ptr [esp+08h],01541F48h
00407AE5  call 01251D5Ch
00407AEA  int3
...
00407AF0  mov eax,[esp+04h]
```

Exact bytes at `0x00407ACE`:

```text
E8 6A A0 E4 00
```

The candidate byte at `0x00407AD0` is therefore `A0`, the second byte of that CALL's signed rel32 displacement. It is not an instruction boundary. The next real instruction begins at `0x00407AD3`.

The surrounding real function is rooted at `0x00407A90`; the next padded/aligned function root begins at `0x00407AF0`.

## Xref evidence

A complete byte-level scan of direct relative control-flow references in the exact `.text` section found:

- `0x00407A90`: one direct CALL from `0x00407E77`;
- `0x00407AD0`: **zero** direct CALL/JMP/Jcc references;
- no raw little-endian pointer value `0x00407AD0` exists anywhere in the executable.

Thus there is no independent entry-point evidence supporting `0x00407AD0`.

## Inventory-policy consequence

The project inventory explicitly permits later candidate classification to remove addresses proven not to be real function starts. The raw V6 union remains 79,782 for audit. With the previously proven false positives `0x00407A10` and `0x00407AB0` already removed, this third evidence-backed correction reduces the current dashboard denominator from 79,780 to **79,779**.

## Scope

- No C++ implementation is created for `FUN_00407AD0`.
- No unit-test target, functions.csv row, claim, FAST_PASS, or FAST_PASS_VALIDATED record is created.
- `FUN_00407A30` and `FUN_00407AB0` are preserved and not modified by this classification.
- No EXE, DLL, proprietary asset, or `.obj` is committed.
