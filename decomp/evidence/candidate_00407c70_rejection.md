# Supplemental address analysis — 0x00407C70

## Candidate-universe membership correction — 2026-09-24

The canonical local `work/analysis/function_candidates.csv` was rechecked and **0x00407C70 is not present in that CSV**. This document is retained only as supplemental binary analysis. All historical wording below that calls this address a V6 candidate, describes it as a removed candidate, or assigns it any denominator reduction is **superseded** and must not be used for accounting. The V6 denominator remains **79,782**.

## Conclusion

0x00407C70 is **not a valid function entry** in the exact Resident Evil 5 PC 1.1.0 Dev target. It is the second byte of the four-byte immediate operand of the five-byte push 0x400 instruction beginning at 0x00407C6F.

This candidate must therefore be removed from the V6 potential-function-start universe rather than represented by fabricated C++, tests, a database/functions.csv row, claim, FAST_PASS, or FAST_PASS_VALIDATED metadata.

## Exact target

- executable SHA-256: 323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815
- size: 19,977,216 bytes
- image base: 0x00400000
- candidate VA: 0x00407C70
- candidate RVA: 0x00007C70

## Instruction-boundary evidence

GNU objdump on the exact local reference decodes the surrounding stream as:

~~~text
00407C53..00407C5F  int3
00407C60  mov  ecx,dword ptr [01652F1Ch]
00407C66  mov  eax,dword ptr [ecx]
00407C68  mov  edx,dword ptr [eax+14h]
00407C6B  push esi
00407C6C  push edi
00407C6D  push 10h
00407C6F  push 400h
00407C74  call edx
00407C76  mov  ecx,dword ptr [esp+10h]
00407C7A  mov  esi,eax
00407C7C  lea  eax,[esp+14h]
...
00407CCC  ret
00407CCD..00407CCF  int3
00407CD0  mov eax,dword ptr [ecx]
~~~

Exact bytes at 0x00407C6F:

~~~text
68 00 04 00 00
~~~

This is the five-byte x86 instruction push 0x00000400. Address 0x00407C70 is byte 2 of that instruction, the first byte of its little-endian immediate, and therefore is not an instruction boundary. The next real instruction begins at 0x00407C74.

LLVM llvm-objdump independently agrees with the same boundaries:

~~~text
00407C6D  6a 10             push 0x10
00407C6F  68 00 04 00 00    push 0x400
00407C74  ff d2             call edx
00407C76  8b 4c 24 10       mov ecx,dword ptr [esp+0x10]
~~~

The containing real aligned function is rooted at 0x00407C60, after INT3 padding at 0x00407C53..0x00407C5F. It runs through the terminal ret at 0x00407CCC; its 109-byte native body 0x00407C60..0x00407CCC has SHA-256 42d47f7c6cc96507eee3e4402437438da1b3cdbcaa5913e1ff57d63e08dcf4c5. The next aligned function begins at 0x00407CD0.

## Xref evidence

A complete decoded direct-relative CALL/JMP/Jcc target search over the exact .text image finds **zero** transfers targeting 0x00407C70.

A whole-file byte scan also finds **zero** raw little-endian pointer values equal to 0x00407C70.

These cross-checks are secondary to the decisive instruction-boundary evidence: the candidate lies inside an already-decoded instruction.

## Inventory-policy consequence

The raw V6 candidate union remains **79,782** for audit. Current integration already contains six evidence-backed removals:

- 0x00407A10
- 0x00407AB0
- 0x00407AD0
- 0x00407BA0
- 0x00407B60
- 0x00407BE0

Adding 0x00407C70 as the seventh proven false positive reduces the evidence-corrected dashboard denominator from **79,776** to **79,775**, while recovered-function and FAST PASS counts do not change.

## Scope

- No C++ implementation is created for FUN_00407C70.
- No tests, functions.csv row, claim, FAST_PASS, or FAST_PASS_VALIDATED record is created.
- FUN_00407C90 and FUN_00407CB0 are excluded parallel work and are not modified.
- Published FUN_00407B10, false-positive corrections 0x00407B60 / 0x00407BE0, and the merged FUN_00407C00 work are preserved.
- No EXE, DLL, proprietary asset, or .obj is committed.
