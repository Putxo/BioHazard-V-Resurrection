# Exact-attachment scalar C++ recovery

This module adds **11,146 readable C++ bodies / 69,445 instruction bytes** recovered
from the same user-supplied PE32 with SHA-256
`323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`
and size **19,977,216 bytes**. No other EXE was analyzed and no EXE, DLL, assets,
object files or other compiled products are published.

The batch is disjoint from the 7,171 published `leaf323d1aab` bodies: the two source
catalogues contain **18,317 distinct addresses**, not 18,317 newly approved canonical
functions. Literal membership in the original 79,782-row CSV remains unverified.
No canonical counters, claims, status tiers or denominator are promoted here.

## Recovered operations

| Family | Bodies |
|---|---:|
| No-op return, including exact callee stack cleanup | 5,145 |
| AL zero / constant return | 33 / 91 |
| Byte field load | 223 |
| Argument stores, 8 / 16 / 32 bits | 56 / 17 / 104 |
| Immediate stores, 8 / 32 bits | 45 / 86 |
| Shift-and-mask extraction, AL / EAX result | 59 / 54 |
| Fixed-address DWORD copy / immediate store | 239 / 4,994 |

The empty C++ no-op bodies correspond to actual original `RET` or `RET n` bodies;
they are not placeholders for unsupported functions. The return-only subset contains
4,883 plain returns and 262 returns that pop 4, 8, 12, 16, 20 or 24 stack bytes.
Unknown original argument names and unused types are not inferred from these returns.
The low-level signatures express observed inputs/results and stack cleanup, not
recovered original class declarations or a complete SDK.

This run independently reconstructs the previously reported scalar families and also
includes two 32-bit-immediate mask helpers at `0x00D3CC60` and `0x00D3DC10`, for a
count two higher than the prior 11,144-body status note. The current generated source,
tests and evidence are the artifacts validated by the reports in this directory.

## Build

The repository root includes this module without replacing existing targets:

```sh
cmake --preset dev
cmake --build --preset dev
ctest --preset dev
```

Standalone Linux source tests:

```sh
cmake -S decomp/scalar323d1aab -B build-scalars -DCMAKE_BUILD_TYPE=Release
cmake --build build-scalars --parallel 2
ctest --test-dir build-scalars --output-on-failure
```

The generated implementation units and separate test catalogue form a static library.
They are **not linked into a reconstructed game executable**. Linux property tests
map private synthetic data at the fixed global addresses using `MAP_FIXED_NOREPLACE`;
they fail rather than overwrite an existing mapping. Only synthetic values are used.
The mappings are non-executable. No original PE is loaded or executed by tests or CI.

## Reproducible verification

- `generated/src/scalars_*.cpp`: 44 ordinary implementation translation units, not dispatch stubs.
- `generated/include/re5/scalars.hpp`: typed declarations with x86 thiscall/stack-pop annotations.
- `generated/catalog.json`: every address, recipe parameters, body size and original-body SHA-256.
- `metadata/semantics.b85`: compressed integer semantic records, not original executable bytes.
- `metadata/calls.b85`: 2,085 direct CALL references for 173 recovered starts.
- `tools/generate.py --check`: reproduce all generated source and catalogue files and compare the recorded digest.
- `tools/recover.py`: redo conservative discovery using the exact local input and GNU objdump.
- `tools/verify_target.py`: verify SHA-256, I386 PE32 mapping, every instruction recipe, both boundary paddings and CALL references.
- `tools/compare_coff.py`: compile every implementation for Windows x86 and compare body bytes locally.

Input validation without execution:

```sh
python decomp/scalar323d1aab/tools/verify_target.py --exe "/path/to/the/attached.exe"
```

Optional independent I386 disassembly with LLVM 19.1:

```sh
python decomp/scalar323d1aab/tools/verify_target.py --exe "/path/to/the/attached.exe" \
  --llvm-library /lib/x86_64-linux-gnu/libLLVM.so.19.1
```

Fresh discovery deliberately refuses to overwrite an existing output report:

```sh
python decomp/scalar323d1aab/tools/recover.py --exe "/path/to/the/attached.exe" \
  --out /tmp/scalar-discovery.json
```

Only complete whitelisted bodies with decoded, consecutive INT3 leading padding,
16-byte alignment and trailing padding are emitted. The early ordered recovery range
below `0x00410000` and the `0x00BB9000..0x00BB90FF` pilot are excluded. Unsupported
instructions, control flow and complex functions are not converted to empty bodies.

## Actual local results and limits

The attached input was rehashed, all **11,146** body recipes and padding boundaries
were rechecked, **2,085** CALL sites were verified, and LLVM independently decoded
**17,789** instructions. A second full GNU discovery reproduced the same records.

GCC Release and Clang 17 ASan/UBSan each passed **4/4 CTests**, with **795,299 property
checks** and **2,576 boundary/read-only checks across 644 memory-access bodies**.
All **11,146** implementations compiled to Windows x86 COFF. With the recorded Clang
17 flags, **10,780 bodies / 66,691 bytes** matched original instruction bytes exactly,
excluding padding. See `reports/local-validation.json` and `reports/coff-summary.json`.
CI independently rebuilds source/tests and x86 objects but never receives the EXE;
the local original-byte comparison is not represented as a CI binary comparison.

The comparisons cover body bytes, not linking, full original ABI/type recovery,
undefined/unused register values, EFLAGS, faults on invalid object pointers, scheduling,
concurrent memory semantics or whole-game behavior. The original I386 process was not
executed. Runtime data initialization, callers, object ownership, engine subsystems,
remaining functions and game linkage still require reconstruction and validation.
A large quantity of tiny leaves does not imply a comparable proportion of game logic.
