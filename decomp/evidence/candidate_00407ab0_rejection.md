# Supplemental address analysis — 0x00407AB0

## Candidate-universe membership correction — 2026-09-24

The canonical local `work/analysis/function_candidates.csv` was rechecked and **0x00407AB0 is not present in that CSV**. This document is retained only as supplemental binary analysis. All historical wording below that calls this address a V6 candidate, describes it as a removed candidate, or assigns it any denominator reduction is **superseded** and must not be used for accounting. The V6 denominator remains **79,782**.

## Conclusion

`0x00407AB0` is **not a valid function entry** in the exact Resident Evil 5 PC 1.1.0 Dev target. It is the final byte of the 5-byte relative `jmp` instruction beginning at `0x00407AAC`.

This candidate must therefore be removed from the V6 potential-function-start universe rather than represented by fabricated C++ or FAST_PASS metadata.

## Exact target

- executable SHA-256: `323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`
- image base: `0x00400000`
- candidate VA: `0x00407AB0`
- candidate RVA: `0x00007AB0`

## Instruction-boundary evidence

LLVM `llvm-objdump` decodes the surrounding native stream as:

```text
00407A90  mov ecx,[esp+04h]
00407A94  sub esp,0Ch
00407A97  test ecx,ecx
00407A99  ja 00407AB1h
00407A9B  xor ecx,ecx
00407A9D  add esp,0Ch
00407AA0  mov dword ptr [esp+08h],10h
00407AA8  mov [esp+04h],ecx
00407AAC  jmp 01251471h
00407AB1  or eax,-1
00407AB4  xor edx,edx
00407AB6  div ecx
00407AB8  cmp eax,1
00407ABB  jae 00407A9Dh
...
00407AEA  int3
...
00407AF0  mov eax,[esp+04h]
```

Exact bytes at `0x00407AAC`:

```text
E9 C0 99 E4 00
```

The byte at `0x00407AB0` is therefore `00`, the fifth byte of that instruction's signed rel32 displacement. It is not an instruction boundary.

The next real instruction begins at `0x00407AB1`, and it is an **internal basic-block target** reached by the short conditional branch at `0x00407A99`.

## Xref evidence

A complete scan of direct relative control-flow references in `.text` found:

- `0x00407A90`: one direct CALL from `0x00407E77`;
- `0x00407AB0`: **zero** direct CALL/JMP/Jcc references;
- `0x00407AB1`: one internal short Jcc reference from `0x00407A99`.

Thus there is no independent control-flow evidence supporting `0x00407AB0` as an entry.

## Repository-policy evidence

`reports/function-inventory.md` states that a candidate function start is enrolled only when it is a **valid decoded instruction** inside `.text`. It also explicitly excludes arbitrary JMP targets because they overwhelmingly represent internal basic blocks.

`reports/function-count-audit.md` states that later classification may prove a candidate is not a real function entry and that, in that case, the canonical denominator may decrease through a versioned, evidence-backed inventory update.

This candidate satisfies that removal condition conclusively.

## Scope

- No C++ implementation is created for `FUN_00407AB0`, because doing so would invent a function absent from the binary.
- No FAST_PASS / FAST_PASS_VALIDATED row is created.
- `FUN_00407A10`, `FUN_00407A30`, and `FUN_00407AD0` are explicitly excluded and not modified.
- No EXE, DLL, proprietary asset, or `.obj` is committed.
