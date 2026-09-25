# Exact-attachment flag and field mutators

This module reconstructs **422 complete address-named C++ bodies / 12,874 original instruction bytes** from only the user-attached PE32 EXE, SHA-256 `323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`, size 19,977,216 bytes. The original PE is never launched or uploaded. There are no empty function placeholders, runtime instruction interpreters or unresolved calls in these selected bodies.

## Operations and ordering

147 scalar setters update flags, store a byte/WORD/DWORD and restore a saved DWORD; 78 scalar setters update flags and store without that snapshot; 111 functions copy a two-DWORD input in the original interleaved order; 33 functions repeat the pair copy into two field groups; 17 functions copy a single source DWORD with flags/snapshot; 36 functions update masked bit ranges. All 422 functions are separately named by original address. Low-level signatures express the observed receiver and one stack argument; original class names and complete source-language types are not inferred.

The source pointer **can overlap the object**. For example, `FUN_00D2D2E0` writes the first destination before reading the second source DWORD. Replacing that sequence with a temporary two-DWORD copy is wrong for overlapping ranges. The double-copy family rereads both input DWORDs for the second destination group, after the first group's writes. Tests include byte-granular overlap with destination, flag and snapshot fields. Snapshot timing, OR masks, shift truncation, exact store width and preservation of unrelated bits are retained. No null checks or early returns are invented for these branchless originals.

`src/mutations.cpp` contains the readable C++ implementations. `records.csv` supplies explicit operands and addresses. At CMake configuration, Python validates those records and creates `records.inc`, which expands each record to its own C++ definition; there is no runtime dispatch or recipe interpreter in the library. The same step emits a readable `body-hashes.csv` in the build directory.

## Original-body evidence

`original-body-manifest.sha256` commits to all independently recorded original body hashes, sizes and ordered names: SHA-256 of the UTF-8 concatenation `symbol,size,body_sha256\n`, without a header. This commitment is pinned in `tools/verify.py`. Every reconstructed instruction recipe must reproduce it, and the separate instruction decoder must recover the same operands, registers and exact `RET 4` cleanup. Altering one body's recipe, address, size or hash changes the commitment. Individual hashes can be regenerated with `--emit` and checked directly against the local EXE with `--exe`.

A full fresh GNU objdump discovery reproduced all 422 records. Selection requires decoded consecutive leading INT3 padding, 16-byte alignment, one complete supported instruction sequence and trailing INT3. Bodies with extra instructions or interior returns are rejected. LLVM independently decoded 3,161 I386 instructions. Exactly **one** direct CALL was found and verified (`0x0104370F` to `0x00D4E4F0`); the other 421 starts are not falsely claimed to have direct-call evidence. Padding selection and test success do not establish membership in the historical canonical CSV.

## Reproduce

```sh
cmake -S decomp/mutator323d1aab -B build-mutators -DCMAKE_BUILD_TYPE=Release
cmake --build build-mutators --parallel 2
ctest --test-dir build-mutators --output-on-failure
python decomp/mutator323d1aab/tools/verify.py --exe /local/path/to/attached.exe
python decomp/mutator323d1aab/tools/recover.py --exe /local/path/to/attached.exe --check
python decomp/mutator323d1aab/tools/run_i386.py --compiler g++ --build-dir build-mutator-i386
```

Local GCC Release and Clang 17 ASan/UBSan each passed **3/3 CTests**, with **602,566 property checks and 2,532 guard/fixture checks** across every body. Tests compare the whole affected object and surrounding bytes, use unaligned receivers/inputs, exercise all low-byte argument values, and cover source/destination aliases. Guard pages bound the receiver and the exact four/eight-byte source reads; separate inputs are read-only. Five intentionally incorrect implementations failed: omitted flags, widened stores, reading the second pair value too early, caching the double-copy input, and dropping a shift mask.

All 422 definitions compiled to Windows I386 COFF. With the recorded local Clang flags, **one body / 23 bytes** matched the original hash exactly; no byte-identical claim is made for the other 421. `tools/check_coff.py` inspects the actual object and excludes relocated ranges.

Both GCC and Clang freestanding ELF32 harnesses compiled locally, but the sandbox rejected native execution with ENOEXEC. This is not a runtime PASS. The GitHub workflow independently runs both compiler harnesses in I386 mode and requires 605,098 checks for each of the C++ and hash-checked instruction-fixture sets. Those selected fixtures operate only on synthetic memory; neither the original PE nor the game is loaded. Inspect the actual CI results before claiming native success. No generated assembly or binaries are committed.

## Scope

The verifier checks address overlap against all existing recovery catalogues and canonical functions/claims available in the checkout. Existing code and metadata are preserved. The historical 79,782-row candidate CSV remains unavailable: no canonical count, denominator, claim or tier is changed. These tests do not prove exact faults for arbitrary invalid/read-only object layouts, incidental return-register values, EFLAGS, concurrent access semantics, original class declarations or whole-game behavior. Full-function coverage, dependency integration and linking a reconstructed game remain unfinished.
