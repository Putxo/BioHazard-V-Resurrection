# Exact-attachment conditional projections

This module recovers **196 complete, separately named C++ bodies / 5,507 original instruction bytes** from only the attached PE32 target, SHA-256 `323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`, size 19,977,216 bytes. It does not launch or upload the original EXE. The recovered library contains no interpreter, placeholders, external helper stubs or embedded machine-code payloads.

## Recovered semantics

There are 40 null-guarded WORD/DWORD loads, 26 nullable address-plus-offset returns, 22 sentinel-controlled reads, 14 unsigned array getters, 21 unsigned array setters, 15 nullable byte/DWORD stores and 58 signed-index/null-guarded nested byte/masked-DWORD readers. Each `include/records.inc` entry expands into a distinct function in `src/projections.cpp`; operands, sizes, exact original-body hashes and decoder variants are also readable in `records.csv`.

The array setter's original argument order is **value first, index second**: `FUN_00539500` reads the index from `[esp+8]`, compares it with `[ecx+0x10]`, and only on success reads the value from `[esp+4]`. Its return pops eight argument bytes. Reversing these arguments is incorrect even though the C++ would still compile. Other preserved details include signed negative counts, 32-bit wrapped index arithmetic, nullable fallbacks that are not dereferenced, unaligned exact-width memory access and sentinel exits before inaccessible result fields. Null `this` guards are not invented.

## Build and checks

```sh
cmake -S decomp/projection323d1aab -B build-projections -DCMAKE_BUILD_TYPE=Release
cmake --build build-projections --parallel 2
ctest --test-dir build-projections --output-on-failure
python decomp/projection323d1aab/tools/verify.py --exe /local/path/to/attached.exe
```

GCC Release and Clang 17 ASan/UBSan each passed 3/3 local CTests: metadata/wrong-input rejection, **177,205 property checks**, and **3,012 guard/fixture checks**. Every body is exercised, including aliasing stores and array pointers preceding their count fields. Four negative controls were rejected: swapped setter arguments, missing null-load guard, unsigned treatment of a negative nested count, and inverted sentinel logic. These are source tests, not game tests.

All 196 bodies compile into Windows I386 COFF. With the recorded local Clang flags, only **6 bodies / 132 bytes** match the original hashes exactly; no exact match is claimed for the other 190. `tools/check_coff.py` inspects actual definitions and excludes relocation-affected ranges.

The exact local input, all 196 ranges and padding boundaries, and **251 direct CALL sites for 44 starts** were rechecked. LLVM 19 independently decoded all **1,792 I386 instructions**. The remaining starts have padding evidence, not invented direct CALL evidence. `tools/verify.py` separately encodes semantic recipes and decodes their register operands, branch destinations and stack cleanup; both must agree with the independently recorded original hashes.

## Native 32-bit source/reference test

```sh
python decomp/projection323d1aab/tools/run_i386.py --compiler g++ --build-dir build-projection-i386
```

This builds a freestanding Linux ELF32 test harness. It runs the C++ implementations and instruction fixtures through the same synthetic-memory properties and guard cases, expecting 180,217 checks for each implementation set. The fixture instructions are generated in the build directory from the audited recipes and checked against the original body hashes before use. There are no external calls or process-specific dereferenced globals in these selected fragments. The original PE, game entry point and assets are not loaded. No fixture binaries or generated assembly are committed.

The local sandbox compiled both GCC and Clang ELF32 harnesses but rejected native I386 execution with `Exec format error`; local compilation is explicitly **not** a native runtime PASS. The new GitHub workflow separately attempts native execution with both compilers. Read its actual job results before claiming success. A failed or unavailable I386 run is not silently skipped.

The x86 calling convention passes the receiver in ECX and lets the callee pop these fixed stack arguments; see the compiler's [x86 attribute documentation](https://gcc.gnu.org/onlinedocs/gcc/x86-Attributes.html). Host x86-64 tests do not establish that ABI. Native instruction-fixture tests add evidence but do not prove complete original class types, arbitrary invalid-pointer fault behavior, EFLAGS/unused-register equivalence, concurrency semantics or whole-game correctness.

## Integration and remaining scope

Root CMake includes the module. Its verifier rejects overlaps with the published leaf/scalar/accessor/guarded catalogues and canonical functions/claims whenever present. Existing source and metadata are not rewritten. The 79,782-row historical CSV was searched for but was not recovered; literal membership in that specific CSV is still not claimed. No canonical denominator, counter, claim or tier is changed. The objective remains all-function recovery and game linkage, which are not completed by this module.
