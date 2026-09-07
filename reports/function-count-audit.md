# Function Count Audit — RE5DX9 1.1.0 Dev Release

Target SHA-256: `323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`

## Current dashboard denominator

**V6 broader reconstructed candidate universe: 79,782 potential function starts**

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

The dashboard denominator is now **79,782 potential function starts**.

During decompilation each candidate must ultimately be classified as a recovered function, thunk/runtime helper, internal basic-block/jump-table target, data misidentified as code, or an ambiguous/blocker entry pending more evidence.

If later classification proves that a candidate is not a real function entry, the canonical denominator may decrease, but only through a versioned, evidence-backed inventory update.

## Exact source-function count remains unavailable

The PE has no COFF function-symbol table or authoritative embedded all-function boundary table. Therefore neither **79,782** nor **79,016** is presented as a mathematically exact original source-symbol count.

For project accounting, **79,782 is now the canonical total candidate universe shown in `main`**.
