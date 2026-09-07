# Full executable re-audit V5 — RE5DX9 1.1.0 Dev Release

Target SHA-256: `323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`

This V5 audit supersedes the V4 working denominator. V4 still relied on linear-disassembly evidence classes that could be generated from embedded data interpreted as code. V5 analyzes the whole PE, classifies every file byte into headers/sections, disassembles all of `.text` with two independent decoders, and then accepts new function roots only when they are reached through a fixed-point control-flow traversal or through strong address-taken evidence.

## Whole-file coverage

The file size is exactly **19,977,216 bytes**. The complete file is accounted for by:

- PE headers: 1,024 raw bytes
- `.text`: 16,185,856 raw bytes, 16,185,403 virtual bytes
- `.rdata`: 2,053,632 raw bytes
- `.data`: 977,920 raw bytes
- `.rsrc`: 758,784 raw bytes

There are no raw-file gaps outside the headers/sections.

PE32 / i386:

- ImageBase: `0x00400000`
- Entry VA: `0x012570A9`
- `.text`: `0x00401000` — `0x0137083B` (exclusive virtual end)
- exports: 0
- imports: 376 across 21 DLLs
- TLS directory: absent
- exception directory: absent
- base relocation directory: absent
- debug directory: RSDS only (`BH5DCRelease.pdb`), matching PDB unavailable

## Full dual-decoder pass

The entire `.text` section was disassembled, not sampled.

- GNU objdump instruction starts: **4,836,558**
- LLVM llvm-objdump instruction starts: **4,836,307**
- starts shared by both: **4,835,646**
- GNU-only starts: 912
- LLVM-only starts: 661

The linear pass also sees very large numbers of apparent CALL/JMP/Jcc instructions because embedded data can decode as x86 instructions. V5 therefore does **not** treat a linear-disassembly reference as function evidence by itself.

## Fixed-point CFG rule

Initial roots are restricted to:

1. 16-byte-aligned instruction starts immediately following compiler `INT3` padding, present in both decoders;
2. PE entrypoint;
3. aligned `.rdata` / `.data` code pointers whose target is a common instruction start and an exact `RET` boundary in both decoders.

The graph is then traversed using LLVM instruction lengths/semantics only at starts that also exist in GNU output. During traversal:

- direct CALL targets are admitted only when the target is agreed by both decoders;
- direct JMP/Jcc control flow is followed only to common instruction starts;
- strict address-taken callbacks (`push`/`mov`/absolute `lea`) are accepted only when the source instruction itself is reachable and the target satisfies the same strong function-boundary rule;
- tail-entry JMP targets are accepted only when their source is reachable and the target is a 16-byte-aligned exact post-RET boundary;
- the process repeats until no new root appears.

The fixed point is reached in two iterations: the second iteration adds **zero** new roots.

## V5 result

### Strong root classes

- post-`INT3` aligned roots: **72,919**
- reachable direct-CALL targets: **16,571**
- CALL additions outside the padding set: **1,669**
- PE-entry-only addition: **1**
- direct/padding base union: **74,589**
- strong address-taken `.rdata` / `.data` additions after CALL deduplication: **4,338**
- strict reachable immediate callback additions: **45**
- strong reachable tail-entry additions: **44**

### Fixed-point high-confidence machine-code entrypoint union

**79,016**

This is **477 fewer** than V4's 79,493. Those 477 V4-only entries are no longer accepted as high-confidence roots because their evidence depended on linear-disassembled regions that are not reached by the fixed-point CFG and/or on looser immediate/table heuristics.

## Reachability

At the fixed point:

- reachable LLVM instructions whose starts are also present in GNU: **3,726,072**
- reachable instruction bytes: **13,772,419**
- `.text` virtual bytes: **16,185,403**
- reachable-code coverage: **85.09%**
- bytes outside the reachable instruction set: **2,412,984**

The non-reached area includes compiler padding, zero-filled areas, embedded constant/table data, decoder disagreement zones and potentially some statically unreachable/dead code. It is therefore intentionally **not** used as automatic function evidence.

## RTTI / virtual dispatch cross-check

Recomputed from the same executable:

- MSVC TypeDescriptors: **433**
- CompleteObjectLocators: **494**
- structural vtables: **494**
- virtual slots: **3,534**
- unique virtual targets: **1,682**

## Section content profile

Raw section statistics:

- `.text`: entropy 6.504356 bits/byte; 1,629,433 zero bytes; 641,002 `0xCC` bytes
- `.rdata`: entropy 6.316767; 435,030 zero bytes; 207,370 aligned dwords that point somewhere into `.text`
- `.data`: entropy 3.487701; 596,083 zero bytes; 7,479 aligned dwords that point somewhere into `.text`
- `.rsrc`: entropy 7.829448; 758,784 raw bytes

A pointer into `.text` is not automatically a function pointer. V5 only promotes pointer targets that also satisfy the strong boundary rule.

## What is and is not proven

`79,016` is the project's **V5 high-confidence machine-code entrypoint denominator** under a reproducible fixed-point CFG rule. It is stronger than V4 because control references from unreachable linear-decoded data cannot create new function roots.

It is still **not** mathematically identical to the number of original C/C++ source functions. The PE contains no COFF function symbols and no authoritative embedded function-boundary table. The matching `BH5DCRelease.pdb` / MAP is unavailable. Without that linker/debug metadata, exact 1:1 source-function counting is impossible from the stripped x86 image alone.

The project must therefore preserve separate concepts:

- **high-confidence machine entrypoints:** 79,016 (V5)
- **ambiguous/unproven possible code entries:** tracked separately, not included in progress
- **exact original source-symbol function count:** unavailable unless the matching PDB/MAP is recovered

## Policy

V5 replaces V4 for progress accounting. Future changes to the denominator require:

1. a new explicit baseline revision;
2. address-level delta evidence;
3. full-PE re-audit;
4. fixed-point CFG convergence;
5. README/report synchronization;
6. CI PASS before merge.
