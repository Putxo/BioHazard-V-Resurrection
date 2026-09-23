# Function Count Audit — RE5DX9 1.1.0 Dev Release

Target SHA-256: `323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`

## Current dashboard denominator

**Current evidence-corrected dashboard universe: 79,777 potential function starts**

This replaces `79,016` as the public total-work denominator so the independently detected 766-candidate difference pool is not silently excluded.

It does **not** mean the original C/C++ source is proven to contain exactly 79,782 functions. The target has no authoritative embedded all-function symbol table and the matching PDB/MAP is unavailable.

## Independent full-scan evidence

A complete independent static pass over the exact target produced:

- complete `.text` linear scan rows: **4,837,889**
- undecodable bytes explicitly retained as data: **662**
- semantic direct `CALL` targets inside code: **19,195**
- targets in aligned sequences of at least two code pointers: **40,912**
- 16-byte-aligned starts after at least two `INT3`/`NOP` padding bytes: **69,076**

Deduplicated union of those three candidate-start evidence classes:

**79,782**

## V5 fixed-point cross-check

The earlier V5 fixed-point CFG method remains useful as a stricter subset/cross-check:

- V5 high-confidence machine-code entries: **79,016**
- V6 broader candidate union: **79,782**
- difference: **766** candidates (**0.97%**)

V5 intentionally rejected roots whose evidence arose only outside its reachable fixed-point graph. The independent full-scan candidate union intentionally keeps broader evidence so those entries remain part of total decompilation work until individually classified.

## Whole executable

The entire **19,977,216-byte** PE is accounted for by headers plus `.text`, `.rdata`, `.data` and `.rsrc`, with zero raw-file bytes left unclassified.

## What the dashboard means

The dashboard denominator is now **79,777 potential function starts**.

During decompilation each candidate must ultimately be classified as one of the project-supported outcomes, for example:

- real recovered function entry;
- thunk/import stub/runtime helper;
- internal basic-block/jump-table target;
- data misidentified as code;
- ambiguous/blocker pending more evidence.

If later classification proves that a candidate is not a real function entry, the canonical denominator may decrease, but only through a versioned, evidence-backed inventory update.

## Evidence-backed candidate correction — 0x00407A10

Candidate `0x00407A10` has been removed from the dashboard denominator after exact-byte classification proved it is not executable function entry code.

- The real switch dispatcher is rooted at `0x00407960`.
- At `0x0040796F` it executes `jmp dword ptr [eax*4+0x00407A0C]` for selector values `0..6`.
- The 28 bytes at `0x00407A0C..0x00407A27` are seven little-endian code pointers: `0x00407976, 0x00407976, 0x00407976, 0x00407986, 0x004079A9, 0x004079B2, 0x004079D6`.
- Therefore `0x00407A10` is the address of the **second four-byte jump-table cell**, whose value is `0x00407976`; it is not a code entry.
- A complete direct `CALL rel32` / direct `JMP rel32` scan finds no branch targeting `0x00407A10`.

This is exactly the kind of per-candidate classification anticipated by the inventory policy: the raw V6 union remains 79,782 for audit, while the current evidence-corrected denominator decreases by one to **79,781**.

## Evidence-backed candidate correction — 0x00407AB0

Candidate `0x00407AB0` has been removed from the dashboard denominator after exact instruction-boundary classification proved it is not executable function entry code.

- `E9 C0 99 E4 00` at `0x00407AAC` is a five-byte near JMP; `0x00407AB0` is its fifth byte, inside the rel32 displacement.
- The next actual instruction is `0x00407AB1`, an internal basic-block target reached from `0x00407A99`.
- A complete direct relative CALL/JMP/Jcc scan finds zero control-flow references to `0x00407AB0`; `0x00407A90` has a direct CALL from `0x00407E77`.

The raw V6 detector output remains 79,782 for audit, while the current dashboard denominator decreases from 79,781 to **79,780**. Full evidence: `decomp/evidence/candidate_00407ab0_rejection.md`.

## Evidence-backed candidate correction — 0x00407AD0

Candidate `0x00407AD0` has been removed from the dashboard denominator after exact instruction-boundary classification proved it is not executable function entry code.

- `E8 6A A0 E4 00` at `0x00407ACE` is a five-byte near CALL to `0x01251B3D`; `0x00407AD0` is inside that instruction's signed rel32 displacement.
- The next actual instruction begins at `0x00407AD3`. The containing real function is rooted at `0x00407A90`, and the next aligned function begins at `0x00407AF0`.
- A complete direct relative CALL/JMP/Jcc scan finds zero control-flow references to `0x00407AD0`, and the executable contains no raw pointer value to that address.

The raw V6 detector output remains 79,782 for audit, while the current dashboard denominator decreases from 79,780 to **79,779**. Full evidence: `decomp/evidence/candidate_00407ad0_rejection.md`.

## Evidence-backed candidate correction — 0x00407BA0

Candidate `0x00407BA0` has been removed from the dashboard denominator after exact instruction-boundary classification proved it is not executable function entry code.

- `F6 44 24 08 01` at `0x00407B9E` is the five-byte instruction `test byte ptr [esp+8], 1`; `0x00407BA0` is its third byte.
- The next actual instruction begins at `0x00407BA3`. The containing real function is rooted at `0x00407B90`, and the next aligned function begins at `0x00407BC0`.
- A complete direct relative CALL/JMP/Jcc scan finds zero control-flow references to `0x00407BA0`, and the executable contains no raw pointer value to that address.

The raw V6 detector output remains 79,782 for audit, while the current dashboard denominator decreases from 79,779 to **79,778**. Full evidence: `decomp/evidence/candidate_00407ba0_rejection.md`.

## Evidence-backed candidate correction — 0x00407B60

Candidate `0x00407B60` has been removed from the dashboard denominator after exact instruction-boundary classification proved it is not executable function entry code.

- The real helper begins at `0x00407B50`.
- `B8 5C AA 37 01` at `0x00407B5E` is `mov eax,0x0137AA5C`; therefore `0x00407B60` is the third byte of that instruction's immediate operand, not an instruction boundary.
- GNU objdump and LLVM llvm-objdump independently agree on the decode.
- A complete direct relative CALL/JMP/Jcc scan finds zero control-flow references to `0x00407B60`, and a whole-file raw-pointer scan finds zero little-endian pointers to that address.
- Real entry `0x00407B50` has many direct callers; next real helper `0x00407B70` also has a direct caller.

The raw V6 detector output remains 79,782 for audit, while the current dashboard denominator decreases from 79,778 to **79,777**. Full evidence: `decomp/evidence/candidate_00407b60_rejection.md`.

## Exact source-function count remains unavailable

The PE has no COFF function-symbol table or authoritative embedded all-function boundary table. Therefore neither **79,782** nor **79,016** is presented as a mathematically exact original source-symbol count.

For project accounting, **79,777 is now the canonical evidence-corrected candidate universe shown in `main`**. The raw V6 detector output of 79,782 remains preserved as an audit baseline.
