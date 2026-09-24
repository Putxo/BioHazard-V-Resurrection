# Supplemental address analysis — 0x00407B60

## Candidate-universe membership correction — 2026-09-24

The canonical local `work/analysis/function_candidates.csv` was rechecked and **0x00407B60 is not present in that CSV**. This document is retained only as supplemental binary analysis. All historical wording below that calls this address a V6 candidate, describes it as a removed candidate, or assigns it any denominator reduction is **superseded** and must not be used for accounting. The V6 denominator remains **79,782**.

## Conclusion

`0x00407B60` is **not a valid function entry** in the exact Resident Evil 5 PC 1.1.0 Dev target.

The surrounding real function begins at `0x00407B50`. At `0x00407B5E` the binary contains the five-byte instruction:

```text
B8 5C AA 37 01    mov eax,0x0137AA5C
```

Therefore `0x00407B60` is the **third byte of that instruction's 32-bit immediate operand**. It is not an instruction boundary and cannot be a callable function entry.

## Exact target

- executable: `9) RE5DX9_1.1.0 Dev (Release 15 des. 2017)(7).exe`
- executable SHA-256: `323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`
- image base: `0x00400000`
- candidate VA: `0x00407B60`
- candidate RVA: `0x00007B60`

## Instruction-boundary evidence

GNU `objdump` and LLVM `llvm-objdump` independently agree on the surrounding stream:

```text
00407B50  mov eax,[ecx+1028h]
00407B56  test eax,eax
00407B58  je 00407B5Eh
00407B5A  add eax,8
00407B5D  ret
00407B5E  mov eax,0137AA5Ch
00407B63  ret
00407B64  int3
...
00407B70  mov eax,[ecx+102Ch]
00407B76  test eax,eax
00407B78  je 00407B7Eh
00407B7A  add eax,8
00407B7D  ret
00407B7E  mov eax,0137AA5Ch
00407B83  ret
```

Exact bytes for the real `0x00407B50` helper:

```text
8B 81 28 10 00 00 85 C0 74 04 83 C0 08 C3 B8 5C AA 37 01 C3
```

The byte at `0x00407B60` is `AA`, byte 3 of the immediate `5C AA 37 01` encoded by the `mov eax,imm32` at `0x00407B5E`.

## Xref evidence

A complete direct relative CALL/JMP/Jcc scan of `.text` finds:

- `0x00407B50`: many direct CALL references, confirming it is a real callable entry;
- `0x00407B60`: **zero** direct CALL/JMP/Jcc references;
- `0x00407B70`: a direct CALL reference, independently confirming the next real helper boundary.

Representative real caller:
- `0x00403923` -> CALL `0x00407B50`.

A raw little-endian pointer scan over the entire executable finds **zero occurrences** of pointer value `0x00407B60`.

Thus there is no direct-control-flow or raw-pointer evidence supporting `0x00407B60` as an entry.

## Repository-policy evidence

The project inventory policy requires candidate starts to be valid decoded instructions. It also explicitly permits evidence-backed removal when later classification proves a candidate is not a real function entry.

This candidate satisfies that removal condition conclusively.

## Scope

- No C++ implementation is created for `FUN_00407B60`.
- No unit test, `database/functions.csv` row, claim, FAST_PASS, or FAST_PASS_VALIDATED state is created.
- `FUN_00407B10` and `FUN_00407BA0` are explicitly excluded and not modified.
- Existing `FUN_00407AF0` work is preserved.
- No EXE, DLL, proprietary asset, or `.obj` is committed.
