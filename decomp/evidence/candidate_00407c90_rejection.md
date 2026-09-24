# Candidate rejection evidence — 0x00407C90

## Conclusion

`0x00407C90` is **not a valid function entry** in the exact Resident Evil 5 PC 1.1.0 Dev target.

Both GNU `objdump` and LLVM `llvm-objdump` decode the bytes spanning the candidate as one instruction beginning three bytes earlier:

```text
00407C8D  8B 7C 24 1C    mov edi,dword ptr [esp+0x1c]
00407C91  8B 07          mov eax,dword ptr [edi]
```

Therefore `0x00407C90` is the fourth byte of the instruction at `0x00407C8D` — the disp8 byte `0x1C` — and is not an instruction boundary. Creating C++, tests, a `database/functions.csv` row, a claim, FAST_PASS, or FAST_PASS_VALIDATED metadata for `FUN_00407C90` would fabricate a function that does not exist in the binary.

## Exact target

- executable: `9) RE5DX9_1.1.0 Dev (Release 15 des. 2017)(7).exe`
- executable SHA-256: `323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`
- size: 19,977,216 bytes
- image base: `0x00400000`
- candidate VA: `0x00407C90`
- candidate RVA: `0x00007C90`

The locally available reference executable was rehashed before this classification and exactly matches the canonical target SHA-256.

## Instruction-boundary evidence

The relevant exact stream is:

```text
00407C80  50                push eax
00407C81  51                push ecx
00407C82  68 00 04 00 00    push 0x400
00407C87  56                push esi
00407C88  E8 0F A2 E4 00    call 0x01251E9C
00407C8D  8B 7C 24 1C       mov edi,dword ptr [esp+0x1c]
00407C91  8B 07             mov eax,dword ptr [edi]
00407C93  83 C4 10          add esp,0x10
00407C96  85 C0             test eax,eax
00407C98  74 18             je 0x00407CB2
```

The four bytes at `0x00407C8D..0x00407C90` are exactly `8B 7C 24 1C`; their SHA-256 is `03dfb30a105e712a2814c1dd94f93aa9e848beedcef08b4fdcdc438616a35669`.

The enclosing real function begins at `0x00407C60`, immediately after INT3 alignment padding at `0x00407C53..0x00407C5F`, and runs through the `ret` at `0x00407CCC`. Its 109-byte body has SHA-256 `42d47f7c6cc96507eee3e4402437438da1b3cdbcaa5913e1ff57d63e08dcf4c5`. The next real function begins at `0x00407CD0` after INT3 bytes at `0x00407CCD..0x00407CCF`.

## Xref evidence

A complete decoded direct relative CALL/JMP/Jcc scan of the exact executable finds:

- `0x00407C90`: **0** direct control-flow references;
- real entry `0x00407C60`: **326** decoded direct references, including `0x00404D66 -> call 0x00407C60`;
- next real entry `0x00407CD0`: **267** decoded direct references, including `0x004037C9 -> call 0x00407CD0`.

A whole-file scan also finds **zero** raw little-endian pointer occurrences of value `0x00407C90`.

The absence of any target reference is secondary evidence; the decisive fact is stronger: `0x00407C90` is physically inside the four-byte instruction rooted at `0x00407C8D`.

## Inventory-policy consequence

The raw V6 union remains **79,782** potential starts for audit. Integration already contains seven evidence-backed removals at `0x00407A10`, `0x00407AB0`, `0x00407AD0`, `0x00407BA0`, `0x00407B60`, `0x00407BE0`, and `0x00407C70`.

This eighth evidence-backed correction reduces the canonical dashboard denominator from **79,775** to **79,774** while leaving the current integration recovered-function count (**124**), recovered-byte count (**22,548 / 15,568,343**) and FAST PASS count (**123**) unchanged.

## Scope

- No C++ implementation is created for `FUN_00407C90`.
- No unit test, `database/functions.csv` row, claim, FAST_PASS, or FAST_PASS_VALIDATED state is created.
- `FUN_00407C70`, `FUN_00407CB0`, and validated `FUN_00407C00` work are preserved and not modified.
- No EXE, DLL, proprietary asset, `.obj`, or binary product is committed.
