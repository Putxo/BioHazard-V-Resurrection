# Exact-attachment sequence and linked-list recovery

This module recovers **42 complete named C++ loop bodies / 2,096 original instruction bytes** from the user-attached RE5 PE32 only: SHA-256 `323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`, size 19,977,216 bytes. The original executable is never launched or uploaded. The library contains normal C++ loops, not an instruction interpreter or external helper placeholders.

## Recovered control flow

Five linked-chain counters, two indexed-node lookups, two terminal-node lookups, three linked accumulators, three zero-terminated DWORD-array counters, three signed-count key searches, thirteen search-and-clear routines, four conditional byte-propagation loops, three fixed-size nullable pointer-array update loops, two nullable-node searches and two predecessor-value searches are reconstructed. Each semantic record in `include/records.inc` defines its own address-named function in `src/sequences.cpp`.

Two important details are intentionally preserved rather than "fixed":

* The `FUN_00C16DC0` family returns index `0xFFFFFFFF` when its signed count is nonpositive or a key is absent, **and still writes zero to the DWORD immediately before the table**. The original uses `[table + index*4]`. Valid fixtures explicitly allocate this preceding slot. No missing-key no-op or extra null guard is invented.
* The `FUN_00573470` family returns immediately when the object's byte already equals the requested value. Otherwise it captures the count **before** updating that byte, but reloads the table pointer on **each** iteration. A child write can alias those fields. Tests independently distinguish captured count from a live count, and reloaded pointer from a cached pointer.

Linked counters/indexed accessors read the next link before advancing the count. Accumulators include the receiver's own value and wrap unsigned DWORD addition. Searches preserve signed versus unsigned count tests, first-match order, null-node skipping where present, and the original stack cleanup. No-op instruction padding is not treated as game logic. Acyclic readable lists and sufficient zero-terminated storage are required where the original assumes them; no arbitrary loop limit or cycle detection is added.

## Reproduce the checks

```sh
cmake -S decomp/sequence323d1aab -B build-sequences -DCMAKE_BUILD_TYPE=Release
cmake --build build-sequences --parallel 2
ctest --test-dir build-sequences --output-on-failure
python decomp/sequence323d1aab/tools/verify.py --exe /local/path/to/attached.exe
python decomp/sequence323d1aab/tools/run_i386.py --compiler g++ --build-dir build-sequence-i386
```

GCC Release and Clang 17 ASan/UBSan locally passed **3/3 CTests each**, covering **773,460 property checks and 748 guard/fixture checks** per compiler across all 42 bodies. Five intentionally incorrect variants were rejected: suppressing the original missing-key write, off-by-one indexed traversal, unsigned treatment of signed counts, caching a mutable table pointer, and rereading the captured count. Protected-page tests cover early returns, first-match exit before inaccessible later slots, nullable nodes, exact-width boundary reads/writes and aliased metadata.

The full input identity, all ranges and padding boundaries, and **173 direct CALL sites for 20 starts** were rechecked. LLVM 19 independently decoded the **765 I386 instructions**; semantic recipes reproduce every locally recorded original-body SHA-256. This is not an assertion that a linear-scan CALL is dynamically reachable. Other selected starts have decoded padding evidence, not invented call references. `records.csv` and `call-sites.csv` retain auditable addresses and hashes without uploading the executable.

All 42 named definitions compiled to Windows I386 COFF. With the recorded Clang flags, **none** matched the original bytes exactly; no MATCHED status is claimed. Readable reconstruction and exact compiler instruction selection are distinct. `tools/check_coff.py` inspects actual compiled definitions and relocation-free ranges.

The native test builds an independent freestanding Linux ELF32 harness, runs C++ and hash-checked recipe-generated instruction fixtures on synthetic memory, and requires 774,208 checks for each implementation set. Neither the original PE nor game assets are loaded. Both GCC and Clang harnesses compiled locally; this sandbox rejected ELF32 execution with `Exec format error`. That local limitation is not a runtime PASS. The read-only CI separately executes both native compiler jobs; their actual results must be checked before claiming native success. The generated reference assembly and all build outputs stay uncommitted.

## Scope and remaining work

Existing leaf/scalar/accessor/guarded/projection catalogues and canonical functions/claims are checked for address overlaps when present. Root CMake adds this module without replacing older targets. No existing function, claim, inventory denominator, counter or tier is changed. The historical 79,782-row CSV was not recovered, so literal membership and canonical tier promotion remain unclaimed. Source recovery does not by itself prove original class declarations, arbitrary-invalid-pointer faults, EFLAGS/unused-register equivalence, concurrency behavior or whole-game correctness. Full decompilation and linking remain unfinished.
