# Exact attachment: source reference and regression verification

This contribution uses only the user-supplied EXE with SHA-256
`323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`
and size **19,977,216 bytes**. It does not execute or upload that EXE.
The repository target identity was checked against the local attachment before writing.

## Scope and integration

The prior attachment-only package recovered `004022A0`, `004022F0`, `00402350`
and `00402420`, covering **142 instruction bytes**. All four addresses already
exist in the repository. Consequently this contribution adds **zero new canonical
functions**, changes no function-count denominator and makes no tier promotion.
It preserves the two original reference C++ files and augments their test suite
with comparisons against the repository's host-adapted string comparator.

The small reference library is deliberately separate from the game's recovered
library: it is a verification aid, not a second canonical implementation to count.
No EXE, DLL, object file or game asset is committed.

## A discrepancy actually reproduced

The original `src/recovered/fun_004022f0.cpp` blob
`3bfac999046e7e6ed45d06b4f817f6c7f33cdd49` used
`std::strcmp(stored, input)` for non-null backing storage. The attached binary
instead compares **input against stored** and normalizes its result to **-1/0/+1**.
At `00402300` it loads an input byte, and at `00402302` compares it to the stored
byte. The `SBB/SBB` pair at `00402339`/`0040233B` derives the normalized result.
See [the locally verified disassembly](evidence/FUN_004022F0.asm.txt).

The new differential test failed on the prior source after four checks, with
`canonical comparator must agree with exact-target reference`. The correction
uses the observed unsigned two-byte loop and early NUL exits; the two pre-existing
unit-test expectations that encoded the reversed ordering are corrected too.
Other functions in that source file are unchanged.

The host adapter's pre-existing null-input/null-text guards, configurable fallback,
public structures and calling convention are intentionally preserved. They are
**not** asserted to be the original binary ABI. The reference C++ member functions
express the observed x86 `thiscall` interface separately. Neither a successful
unit test nor object compilation establishes whole-game equivalence.

## Build the reference and regression suite

From the repository root:

```sh
cmake -S verification/attachment-323d1aab -B build-attachment -DCMAKE_BUILD_TYPE=Release
cmake --build build-attachment --parallel 2
ctest --test-dir build-attachment --output-on-failure
```

The suite checks the four recovered reference routines, the canonical comparator,
existing string-wrapper tests, early exits against inaccessible guard pages on
POSIX, source-only hygiene, and rejection of wrong or missing input files. Assertions
in the pre-existing tests are explicitly kept active in Release builds.

To verify the **local** authorized EXE without executing it:

```sh
python verification/attachment-323d1aab/verify_target.py --exe "/path/to/the/attached.exe"
```

The verifier checks the full file identity, x86 PE32 headers, address-to-file
mapping, all four instruction-range hashes, and the empty fallback's first byte.
CI does not receive the proprietary EXE and therefore cannot rerun that local check.

## Evidence and provenance

- `target.json`: exact attachment identity and locally rechecked instruction ranges.
- `evidence/`: static disassembly of the four routines, not an EXE or a code dump of the game.
- `local_validation.json`: local test results, source hashes and explicit limitations.
- `origin.json`: SHA-256 inventory of the prior downloadable package for traceability.

Large exploratory call-target CSVs and historical reports from the initial package
are not imported into canonical databases. Their hashes remain in `origin.json`.
They do not supersede the repository's more developed inventory. The original
source/header and four instruction evidence files are preserved byte-for-byte.
The package's obsolete "GitHub destination unknown" reports are not republished as
current state.

## Remaining goal

The objective remains a standalone Windows x86 game executable built from recovered
C/C++ and linked dependencies, using the user's original game data. **This contribution
builds tests and static libraries only. It does not rebuild or run Resident Evil 5.**
No full-game completion percentage, runtime validation, byte match or linked status
is claimed by this package. Existing canonical progress is not reset or inflated.
