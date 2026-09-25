# Conditional accessor recovery — exact attachment 323d1aab

This module reconstructs **127 complete C++ function bodies / 4,222 instruction bytes** from the exclusively authorized EXE (19,977,216 bytes; SHA-256 `323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`). No original executable, DLL, asset or compiled product is included.

## Behavior recovered

There are **39 flag setters**, **39 unsigned bounds-checked array getters**, and **49 signed-index/null-checked nested getters**. `include/records.inc` supplies exact addresses and operands to the readable bodies in `src/accessors.cpp`. Each row expands to a distinct, externally named function definition. There is no runtime dispatch interpreter or placeholder implementation in the recovered library.

Flag setters use the low byte of the argument, preserving unrelated bits. Array getters compare the index and count as unsigned values before reading the backing pointer; they do not invent a null-pointer guard on the success path. Nested getters preserve the original check order: table null, negative index, signed count bound, entry null, then the final field read. Raw stored pointers and indexed address arithmetic are explicitly 32-bit, including modulo-2^32 index multiplication.

## Evidence and reproducibility

`records.csv` identifies every body by exact address, operands, size and direct CALL sites. `body-hashes.csv` retains the SHA-256 hashes taken from the original ranges. Instruction recipes reproduce those independently recorded hashes; no original machine-code fragments are stored as executable payloads. Selection required decoded consecutive leading INT3 padding, a 16-byte-aligned start, a complete supported body and trailing INT3. The early ordered range and BB90 pilot are excluded.

The verifier independently parses the complete supported instruction sequences, including register operands, signed immediates, branch destinations and exact RET stack cleanup. It checks that these semantics equal the C++ source parameters. The local target verification checked all 127 ranges/boundaries and **145 direct CALL references for 29 starts**; the other starts are not claimed to have direct CALL evidence. LLVM 19 independently decoded **1,281 I386 instructions**.

```sh
cmake -S decomp/accessor323d1aab -B build-accessors -DCMAKE_BUILD_TYPE=Release
cmake --build build-accessors --parallel 2
ctest --test-dir build-accessors --output-on-failure
python decomp/accessor323d1aab/tools/verify.py --exe /local/path/to/attached.exe
```

Optional independent decoding: append `--llvm /lib/x86_64-linux-gnu/libLLVM.so.19.1`. The EXE stays local and is never executed by these tools. CI verifies the source and metadata, not the unavailable original binary.

## Tests and limits

GCC Release and Clang 17 ASan/UBSan each passed **3/3 CTests**: metadata/input-rejection, **366,888 property checks**, and **1,566 guard/fixture checks**. Guard tests exercise every body with protected memory, including the array whose pointer field precedes its count. Fixtures cover unsigned high-bit indices and wrapped index multiplication, negative signed counts/indices, null tables/entries, and final DWORD accesses against inaccessible page boundaries. Synthetic low-address mappings never overwrite existing mappings. See `reports/local-validation.json` for exact source hashes.

All **127 named functions compiled to Windows i386 COFF**. This proves compilation, not execution of the original 32-bit game or byte-for-byte matching. No i386 execution, full original class/type recovery, EFLAGS equivalence, invalid-pointer fault equivalence, concurrency model, game linkage or whole-game correctness is claimed. Host x86-64 property/guard tests are source tests, not native execution of the original instructions. Stored 32-bit addresses must denote valid original-layout objects on reachable paths.

## Canonical inventory

These branch-containing bodies are distinct from the leaf and scalar batches; repository checks reject address overlaps where those catalogues exist. The original 79,782-row canonical CSV has not been supplied to this module, so membership remains unverified and **no canonical function/tier counter is promoted**. This contribution neither changes the 79,782 denominator nor overwrites existing function/claim rows. The complete decompilation and game linkage remain unfinished.
