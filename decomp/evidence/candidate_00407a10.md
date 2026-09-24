# Supplemental address analysis — 0x00407A10

## Candidate-universe membership correction — 2026-09-24

The canonical local `work/analysis/function_candidates.csv` was rechecked and **0x00407A10 is not a row in that CSV**. This file is retained only as supplemental binary/address analysis. Any earlier wording that described 0x00407A10 as a V6 candidate, a removed candidate, or a denominator decrement is superseded. This address has **no effect** on the 79,782-row V6 dashboard denominator.

## Result

`0x00407A10` is **not a function entry** in the exact reference executable. It is embedded switch-table data belonging to the real function rooted at `0x00407960`.

No C++ implementation, unit-test target, `database/functions.csv` row, FAST_PASS flag, or FAST_PASS_VALIDATED promotion is created for this address because doing so would knowingly classify data as executable code.

## Target identity

- Reference executable: `/mnt/data/re5_decomp/re5dx9_original.exe`
- SHA-256: `323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`
- Size: 19,977,216 bytes
- PE32 x86 image base: `0x00400000`
- Candidate VA: `0x00407A10`, RVA `0x00007A10`

## Containing switch dispatcher

```text
00407960  56                    push esi
00407961  8B F1                 mov  esi,ecx
00407963  8B 46 28              mov  eax,[esi+28h]
00407966  83 F8 06              cmp  eax,6
00407969  0F 87 98 00 00 00     ja   00407A07
0040796F  FF 24 85 0C 7A 40 00  jmp  dword ptr [eax*4+00407A0Ch]
...
00407A07  5E                    pop esi
00407A08  C3                    ret
```

The indexed branch instruction explicitly names `0x00407A0C` as the base of a seven-entry pointer table.

## Exact jump-table bytes

Range `0x00407A0C..0x00407A27` (28 bytes):

`76 79 40 00 76 79 40 00 76 79 40 00 86 79 40 00 A9 79 40 00 B2 79 40 00 D6 79 40 00`

SHA-256 of those 28 table bytes:

`b1199e15505de3e2d87d6a7f3ebdaed777e1d54e872afab8369576474ec00573`

Decoded little-endian entries:

| selector | cell VA | target VA |
|---:|---:|---:|
| 0 | `0x00407A0C` | `0x00407976` |
| 1 | `0x00407A10` | `0x00407976` |
| 2 | `0x00407A14` | `0x00407976` |
| 3 | `0x00407A18` | `0x00407986` |
| 4 | `0x00407A1C` | `0x004079A9` |
| 5 | `0x00407A20` | `0x004079B2` |
| 6 | `0x00407A24` | `0x004079D6` |

Thus the analyzed address address itself is the storage address of table entry 1. Its four bytes are `76 79 40 00`, i.e. the pointer value `0x00407976`.

## Xref / entry-point checks

A complete direct-relative branch scan of the exact `.text` section finds:

- direct `CALL rel32` targets to `0x00407A10`: **0**
- direct `JMP rel32` targets to `0x00407A10`: **0**

The exact executable also contains no raw little-endian pointer value `0x00407A10` elsewhere. The only semantic use of this address is as the second cell selected by the indexed jump through table base `0x00407A0C`.

## Candidate-universe accounting

0x00407A10 is absent from the canonical local `function_candidates.csv`. Its byte-level classification remains useful, but it is not a false-positive removal from the V6 universe and does not change the denominator. The canonical denominator remains **79,782**.

## Scope exclusions

This classification is exclusive to candidate `0x00407A10`. `FUN_00407A30`, `FUN_00407AB0`, and `FUN_00407AD0` are not analyzed, implemented, or modified here.
