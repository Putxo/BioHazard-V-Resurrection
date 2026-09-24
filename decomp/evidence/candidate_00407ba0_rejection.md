# Supplemental address analysis — 0x00407BA0

## Candidate-universe membership correction — 2026-09-24

The canonical local `work/analysis/function_candidates.csv` was rechecked and **0x00407BA0 is not present in that CSV**. This document is retained only as supplemental binary analysis. All historical wording below that calls this address a V6 candidate, describes it as a removed candidate, or assigns it any denominator reduction is **superseded** and must not be used for accounting. The V6 denominator remains **79,782**.

## Conclusion

`0x00407BA0` is **not a valid function entry** in the exact Resident Evil 5 PC 1.1.0 Dev target. It is a byte inside the five-byte `test byte ptr [esp+8], 1` instruction beginning at `0x00407B9E`.

This candidate must therefore be removed from the V6 potential-function-start universe rather than represented by fabricated C++, tests, a `database/functions.csv` row, claim, or FAST_PASS metadata.

## Exact target

- executable SHA-256: `323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`
- size: 19,977,216 bytes
- image base: `0x00400000`
- candidate VA: `0x00407BA0`
- candidate RVA: `0x00007BA0`

## Instruction-boundary evidence

GNU `objdump` on the exact local reference decodes the surrounding native stream as:

```text
00407B90  push esi
00407B91  mov  esi,ecx
00407B93  mov  dword ptr [esi],01541F48h
00407B99  call 01251BE3h
00407B9E  test byte ptr [esp+08h],01h
00407BA3  je   00407BAEh
00407BA5  push esi
00407BA6  call 0125145Ch
00407BAB  add  esp,04h
00407BAE  mov  eax,esi
00407BB0  pop  esi
00407BB1  ret  4
00407BB4  int3
...
00407BC0  mov  eax,[ecx]
```

Exact bytes at `0x00407B9E`:

```text
F6 44 24 08 01
```

The candidate byte at `0x00407BA0` is therefore `24`, the third byte of that TEST instruction. It is not an instruction boundary. The next real instruction begins at `0x00407BA3`.

The surrounding real function is rooted at `0x00407B90`, has a 36-byte native body through `ret 4` at `0x00407BB1`, and SHA-256 `813a12ff2bad312a13ec8e4d232129d6dc5836a88eb679f39431a793d829f721`. The next aligned function root begins at `0x00407BC0`.

## Xref evidence

A complete byte-level scan of direct relative control-flow references in the exact `.text` section found:

- direct CALL/JMP/Jcc targets to `0x00407BA0`: **0**
- raw little-endian pointer values `0x00407BA0` anywhere in the executable: **0**

There is therefore no independent control-flow or pointer evidence supporting `0x00407BA0` as an entry.

## Inventory-policy consequence

The project inventory permits later candidate classification to remove addresses proven not to be real function starts. The raw V6 union remains 79,782 for audit. With the previously published false positives `0x00407A10`, `0x00407AB0`, and `0x00407AD0` already removed, this fourth evidence-backed correction reduces the current dashboard denominator from 79,779 to **79,778**.

## Scope

- No C++ implementation is created for `FUN_00407BA0`.
- No unit-test target, functions.csv row, claim, FAST_PASS, or FAST_PASS_VALIDATED record is created.
- `FUN_00407B10` and `FUN_00407B60` are preserved and not modified.
- Existing published `FUN_00407AF0` work is preserved.
- No EXE, DLL, proprietary asset, or `.obj` is committed.
