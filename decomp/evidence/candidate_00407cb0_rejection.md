# Candidate rejection evidence — 0x00407CB0

## Conclusion

`0x00407CB0` is **not a valid function entry** in the exact Resident Evil 5 PC 1.1.0 Dev target.

Unlike candidates that land inside instruction bytes, `0x00407CB0` is a genuine instruction boundary. Both GNU `objdump` and LLVM `llvm-objdump` decode the two bytes at that address as the indirect instruction `call eax`. Exact surrounding control flow proves that instruction is an internal operation inside the real function rooted at `0x00407C60`, not a separately callable function.

Creating C++, tests, a `database/functions.csv` row, a claim, FAST_PASS, or FAST_PASS_VALIDATED metadata for `FUN_00407CB0` would therefore split one native function and fabricate an entry absent from the binary.

## Exact target

- executable: `9) RE5DX9_1.1.0 Dev (Release 15 des. 2017)(7).exe`
- executable SHA-256: `323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`
- size: 19,977,216 bytes
- image base: `0x00400000`
- candidate VA: `0x00407CB0`
- candidate RVA: `0x00007CB0`

The locally available reference executable was rehashed before classification and exactly matches the canonical target SHA-256 above.

## Instruction and control-flow evidence

GNU `objdump` decodes the relevant native stream as:

```text
00407C60  mov  ecx,dword ptr [01652F1Ch]
00407C66  mov  eax,dword ptr [ecx]
00407C68  mov  edx,dword ptr [eax+14h]
00407C6B  push esi
00407C6C  push edi
00407C6D  push 10h
00407C6F  push 400h
00407C74  call edx
...
00407C8D  mov  edi,dword ptr [esp+1Ch]
00407C91  mov  eax,dword ptr [edi]
00407C93  add  esp,10h
00407C96  test eax,eax
00407C98  je   00407CB2h
00407C9A  add  dword ptr [eax],-1
00407C9D  mov  eax,dword ptr [edi]
00407C9F  cmp  dword ptr [eax],0
00407CA2  jne  00407CB2h
00407CA4  mov  ecx,dword ptr [01652F28h]
00407CAA  mov  edx,dword ptr [ecx]
00407CAC  push eax
00407CAD  mov  eax,dword ptr [edx+18h]
00407CB0  call eax
00407CB2  push esi
00407CB3  mov  ecx,edi
00407CB5  call 00402360h
00407CBA  mov  ecx,dword ptr [01652F1Ch]
00407CC0  mov  edx,dword ptr [ecx]
00407CC2  mov  eax,dword ptr [edx+18h]
00407CC5  push esi
00407CC6  call eax
00407CC8  mov  eax,edi
00407CCA  pop  edi
00407CCB  pop  esi
00407CCC  ret
00407CCD..00407CCF  int3
00407CD0  mov  eax,dword ptr [ecx]
```

LLVM `llvm-objdump` independently agrees on the candidate boundary:

```text
00407CA4  8b 0d 28 2f 65 01   mov ecx,dword ptr [0x1652f28]
00407CAA  8b 11               mov edx,dword ptr [ecx]
00407CAC  50                  push eax
00407CAD  8b 42 18            mov eax,dword ptr [edx+0x18]
00407CB0  ff d0               call eax
00407CB2  56                  push esi
00407CB3  8b cf               mov ecx,edi
00407CB5  e8 a6 a6 ff ff      call 0x00402360
```

The decisive structural facts are:

- `0x00407CB0` is reached by normal fallthrough after loading the indirect target from `[edx+0x18]`;
- earlier branches at `0x00407C98` and `0x00407CA2` skip that internal call and rejoin at `0x00407CB2`;
- execution after the candidate also falls through directly to `0x00407CB2`;
- all paths then continue inside the same function through its terminal `ret` at `0x00407CCC`.

Thus `0x00407CB0` is an **internal indirect-call instruction**, not a function root.

The real containing function begins at `0x00407C60`, immediately after INT3 alignment padding, and its 109-byte body `0x00407C60..0x00407CCC` has SHA-256:

```text
42d47f7c6cc96507eee3e4402437438da1b3cdbcaa5913e1ff57d63e08dcf4c5
```

The candidate bytes are exactly:

```text
FF D0
```

Their SHA-256 is:

```text
491e503ba40e8d303d46b4b51b14826ae08e8c8c3ded6f1441ef1d5d53c42534
```

## Xref evidence

A complete decoded direct-relative CALL/JMP/Jcc target scan over the exact executable finds:

- `0x00407CB0`: **0** direct control-flow references;
- real entry `0x00407C60`: **326** decoded direct references, including `0x00404D66 -> call 0x00407C60`.

A whole-file raw-byte scan finds **zero** little-endian pointer occurrences of value `0x00407CB0`.

The lack of entry references is secondary to the control-flow proof, but it independently supports the same classification.

## Inventory-policy consequence

The raw V6 candidate union remains **79,782** potential starts for audit. Current integration already contains eight evidence-backed removals, including the independently merged `0x00407C70` and `0x00407C90` corrections.

Adding `0x00407CB0` as the ninth proven false positive reduces the evidence-corrected dashboard denominator from **79,774** to **79,773**, while recovered-function count, recovered-byte count, and FAST PASS count remain unchanged.

## Scope

- No C++ implementation is created for `FUN_00407CB0`.
- No unit test, `database/functions.csv` row, claim, FAST_PASS, or FAST_PASS_VALIDATED record is created.
- The independently merged `0x00407C70` and `0x00407C90` corrections are preserved and not modified.
- The merged and validated `FUN_00407C00` work is preserved.
- No EXE, DLL, proprietary asset, `.obj`, or binary product is committed.
