# Exact-attachment bit-preserving transfers

This module reconstructs **201 complete address-named C++ bodies / 5,470 original instruction bytes** from only the attached PE32 EXE, SHA-256 `323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`, size 19,977,216 bytes. No original executable or game asset is uploaded or launched. The recovered library contains C++ operations, not an instruction interpreter or placeholder functions.

## Reconstructed operations

There are 58 scalar bit stores, 56 flag/snapshot scalar transfers, 18 flag/scalar transfers, 10 indirect scalar stores, 2 duplicated stores, 5 duplicate-and-zero helpers, 3 scalar-plus-byte setters, 21 captured WORD-pair output copies, 12 captured DWORD-pair output copies and 16 guarded nested WORD-pair outputs. Each record produces one separately named C++ function through `records.inc`, generated at configuration time from the audited CSV. There is no runtime recipe dispatch.

The scalar originals use MOVSS as a data move, not a floating-point calculation. The low-level C++ interface accepts the four stack bytes as `u32 bits` and copies them unchanged. This avoids numerical conversions or flushing small values. Tests include negative zero, subnormal bit patterns, infinities and signaling/quiet NaN payloads, under all four tested MXCSR FTZ/DAZ combinations. The tests also require MXCSR to remain unchanged. Original high-level parameter types and class names are not inferred from the instruction sequence.

The paired-output functions **read both input values before either output write**. Their outputs can overlap either source field, including unaligned and partial overlaps. This is deliberately different from the interleaved input copies in the earlier mutator module. The nested variants preserve the exact check order: null owner, negative index, signed count bound, null node, then two WORD reads. Their fallback writes two recorded constants; it is not a silent no-op. Null `this` or output guards are not invented. Stored addresses and indexed calculations retain their 32-bit wrapping behavior.

## Evidence and reproduction

`records.csv` stores all addresses, semantic operands, body sizes and independently recorded original hashes. `tools/model.py` separately parses and rebuilds the closed instruction grammar, checking registers, memory widths, branch destinations and RET 4. Both results must agree with the hash-pinned CSV. Every original body and its padding boundaries were rechecked against the local EXE. Fresh GNU objdump discovery reproduced the entire set. LLVM 19 independently decoded **1,206 I386 instructions**. The local scan found **41 direct CALL sites for 9 starts**; no direct-call evidence is invented for the other starts, and static CALL references are not claims of dynamic reachability.

```sh
cmake -S decomp/transfer323d1aab -B build-transfers -DCMAKE_BUILD_TYPE=Release
cmake --build build-transfers --parallel 2
ctest --test-dir build-transfers --output-on-failure
python decomp/transfer323d1aab/tools/verify.py --exe /local/path/to/attached.exe
python decomp/transfer323d1aab/tools/recover.py --exe /local/path/to/attached.exe --check
python decomp/transfer323d1aab/tools/run_i386.py --compiler g++ --build-dir build-transfer-i386
```

Local GCC 14.2 Release and Clang 17 ASan/UBSan each passed **3/3 CTests**, including **197,870 property checks and 2,448 guard/fixture checks**. Every body is exercised. Protected memory checks cover exact input/output widths and each nested early-exit stage. Five deliberately incorrect implementations were rejected: flushed raw subnormal/negative-zero bits, missing dirty flags, a late second pair read, widened WORD outputs and an unsigned nested-count check. Their actual assertion failures are retained in the local report.

All **201** named definitions compiled for Windows I386. With the recorded local Clang flags, **zero** bodies matched the original instruction bytes; no MATCHED status is claimed. The source interface is a recovered low-level contract, not a guarantee of identical compiler code generation or a linked Windows game.

Both freestanding GCC and Clang ELF32 harnesses compiled locally. The sandbox rejected GCC ELF32 execution with ENOEXEC; Clang ELF32 was build-only. Neither is claimed as a local native runtime PASS. The separate read-only CI jobs execute both C++ and original-hash instruction fixtures with GCC and Clang, requiring **200,318 checks per implementation set**. Those generated fixtures use synthetic memory and do not load the original PE. Read their actual job results before claiming native success. Generated assembly and all compiled products remain outside the committed source.

## Integration scope

The verifier rejects overlap with all available recovery catalogues and canonical function/claim rows. Full-checkout overlap validation runs in CI; the local validation workspace contains this new module only. Root CMake adds this module without replacing older targets. Existing canonical data, claims, counters, tiers and the 79,782 denominator are untouched. Literal membership in the unavailable historical candidate CSV remains unverified. No whole-game linkage/completion, incidental register or EFLAGS equivalence, arbitrary-invalid-memory fault equivalence, original class declaration or concurrency equivalence is asserted.
