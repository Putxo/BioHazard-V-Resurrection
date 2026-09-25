# Guarded body recovery - exact attached EXE 323d1aab

This module adds **146 complete C++ bodies / 4,061 original instruction bytes** from the exclusively authorized local EXE: 19,977,216 bytes, SHA-256 `323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`. The original EXE is never executed or uploaded. No DLL, asset, compiled object or executable payload is distributed.

## Recovered behavior

There are **59 conditional byte stores**, **56 null-guarded initializers** and **31 guarded bitfield readers**. Each explicit address/operand record expands into a separately named complete C++ function. There are no placeholder calls or runtime instruction interpreters.

Stores read the nested pointer, test one byte with its observed mask, and write exactly one byte only on the taken path. They do not invent a null-pointer guard. Initializers preserve the null early return, the original WORD-before-DWORD write order and the returned input pointer in EAX. Bitfield readers preserve table-null, negative signed index, signed count bound and entry-null guards before the final field read; each returns its actual fallback constant, then uses the observed logical shift and AND mask on success. Stored addresses and nested arithmetic are explicitly 32-bit. The initializers use cdecl; the other families use thiscall on I386.

## Evidence and tests

`records.csv` retains each original address, parameters, body length and independently calculated original SHA-256. Selection required a 16-byte-aligned start after at least two decoded INT3 padding bytes, a complete supported body and trailing INT3. `tools/verify.py` separately decodes the supported opcodes/registers and branch targets, checks recipe hashes and source declarations, rejects corrupted branches/truncated bodies and refuses any other EXE. All 146 original ranges and boundaries were checked locally. No direct CALL references were found for this batch; no call evidence is invented.

GCC Release and Clang 17 with ASan/UBSan each passed **3/3 CTests**, **1,212,199 property checks** and **1,577 guard/fixture checks**. Tests cover all 146 bodies, all byte flags, negative and boundary indices/counts, wrapped indexing, null input/table/entry, exact fallbacks, read-only untaken paths and final accessible byte/WORD/DWORD boundaries. Assertions are not compiled out: test requirements remain active in Release. Three negative-control implementations were rejected: unconditional byte store, removed initializer null guard and wrong fallback constant.

All 146 named definitions compiled into a real Windows I386 COFF object with Clang 17. **115 compiled bodies / 2,356 instruction bytes were identical to the independently recorded originals**, excluding padding and bodies containing relocations: all 59 stores and 56 initializers. The other 31 bodies compile and pass source tests but are not byte-identical. `tools/check_i386.py` repeats this comparison on an actual supplied object. Exact-match counts may vary with compiler versions; the checker reports observed results rather than declaring a fixed expected match count. LLVM 19 independently decoded 1,158 I386 instructions from the locally supplied original ranges.

## Reproduction

From the repository root:

```sh
cmake -S decomp/guarded323d1aab -B build-guarded -DCMAKE_BUILD_TYPE=Release
cmake --build build-guarded --parallel 2
ctest --test-dir build-guarded --output-on-failure
python decomp/guarded323d1aab/tools/verify.py --exe /local/path/to/attached.exe
clang++ --target=i686-pc-windows-msvc -std=c++17 -O2 -ffreestanding -fno-exceptions -fno-rtti -Idecomp/guarded323d1aab/include -c decomp/guarded323d1aab/src/guarded.cpp -o build-guarded/guarded-i386.obj
python decomp/guarded323d1aab/tools/check_i386.py build-guarded/guarded-i386.obj
```

CI receives source only. It validates the hashes of locally tested files, repeats both compiler suites and compiles/checks I386 definitions. It does not receive or revalidate the original proprietary EXE.

## Scope and integration limits

Existing leaf, scalar, accessor and canonical function catalogues are checked for overlapping addresses when present. The historical 79,782-row candidate CSV is not available to this module: canonical membership is not asserted and **no canonical counter, denominator or tier is changed**. Original source-level classes, pointer ownership and whole-program call graphs remain unresolved. These tests do not establish invalid-pointer fault ordering, multithreaded behavior, EFLAGS preservation, native I386 execution or whole-game equivalence. No function is promoted to the repository's MATCHED tier by this contribution.

The whole-project CMake includes this additional source/test module without replacing existing implementations. It builds recovered libraries and tests, **not a linked or playable Resident Evil 5 executable**. The full decompilation remains unfinished.
