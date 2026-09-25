# FUN_00408700: evidence-backed provisional C++ candidate

**Status: PROVISIONAL_LOCAL_TESTS_ONLY. Not ready for recovery merge or
FAST_PASS_VALIDATED promotion. No native EXE was rehashed in this session.**

This is a continuation on `decomp/00408700-next`, not a new reservation or
another function. The original `decomp/evidence/fun_00408700.preflight.md`
checkpoint is retained unchanged as historical evidence. This document updates
its pending items; in particular the literal candidate row is now supplied.

## Canonical membership

The user confirmed the literal row:

```text
34560,4228864,1,direct CALL target (linear sweep)
```

`4228864 == 0x00408700` and `34560 == 0x00008700`; the image base is
`0x00400000`. Accept this supplied row for coordination. Do not demand the CSV
again to prove the coordinates, and do not claim an independent whole-CSV read.
The denominator remains **79,782**. No false-positive classification is made.

## Remote state used for resumption

- Original exclusive branch head: `70349f8f4214ae1e553587908140465c2ee0646b`.
- Main observed: `184a9d7ea3a858e1fdf364de8a2d1f6ddbc71737`, the same immutable
  snapshot whose functions/claims were read in the previous preflight.
- Integration observed: `53466552e4f87ccaec42d485e5cfaac90210ffde`.
- Comparison: the exclusive branch had only its original 125-line checkpoint;
  integration had advanced by 18 commits, including the parallel 86A0 work.
- Updated integration functions/claims were read, with no `00408700` occurrence.
- The branch search returned only `decomp/00408700-next`. The all-state PR
  search for `00408700` returned zero results. Other open PRs are unrelated.

Before the write, integration advanced again to
`b2a92926745ab1bcf3ffb006dea34a16516189ba` and main to
`9f9af9693fb14baa0a1505b98bbbe731e4b329b7`. Their deltas were inspected:
the two new integration commits do not change functions/claims; main's two
added inventory/claim entries are 8620 and 86A0, not 8700. The corresponding
main tail rows were read. The exclusive branch remained at its original SHA.

The continuation keeps the original branch history and includes the newer
integration snapshot without overwriting its parallel files. This is a branch
synchronization, not a recovery merge into integration.

## Evidence and what the candidate implements

The wrapper summary is in `decomp/evidence/region_00408000_00409800.md` at
main commit `184a9d7ea3a858e1fdf364de8a2d1f6ddbc71737`: call `0x00A712D0`,
install `0x0137E470`, return the original this pointer.

The new shared-base evidence is `decomp/evidence/fun_004086a0.md` at integration
commit `53466552e4f87ccaec42d485e5cfaac90210ffde`, blob
`c49d8324ca012bfaf9d48dd5c6b742f348a7cbc3`. It documents the base's call to
`0x00A67E10`, zero writes at +04/+08/+0C, sentinel at +10, unchanged +14,
and literal movss bits `0x447A0000` at +18.

`candidate.cpp` is original portable C++ implementing that documented state
transition and the 00408700-specific final vtable. It does not call 86A0 or
install 86A0's derived vtable. The leading 0x1C-byte `Prefix` is a documented
base prefix, **not a claim that 00408700 allocates exactly 0x1C bytes**.
Raw scalar bits are retained without introducing host floating-point changes.
No class names, extra fields, callers, native body bytes or body size are guessed.

The portable null guard is deliberately host-only. The constructor's native
branch/exception/ABI behavior still requires direct verification. Tests are
self-consistency checks of this evidence-backed candidate, not a native oracle
or a proof of instruction equivalence.

## Validation actually performed

Seven named cases passed under each of GCC Debug, GCC Release (checks remain
active under NDEBUG), and Clang AddressSanitizer + UndefinedBehaviorSanitizer:
**21 successful executions, zero failures**. Cases cover the host null guard,
known writes and pointer return, preserved +14, surrounding guard regions,
reconstruction of an already initialized object, independent objects, and
all 256 uniform input-byte patterns.

Four negative controls were compiled locally and rejected by the tests: wrong
final vtable, clobbered +14, omitted +04 initialization, and an invented early
return for an already initialized object. These controls do not imply native
validation. Mutated sources and build products are not committed.

See `local_validation.json` for source hashes, compiler versions, log hashes
and observed negative-control exits. No canonical full-repository build or
GitHub CI run is claimed for this candidate. The standalone CMake project is
intentionally not wired into canonical recovered sources or progress metrics.

Run from the repository root, using an out-of-tree local build directory:

```sh
cmake -S decomp/provisional/fun_00408700 -B /tmp/re5-00408700 -DCMAKE_BUILD_TYPE=Release
cmake --build /tmp/re5-00408700
ctest --test-dir /tmp/re5-00408700 --output-on-failure
```

## Exact input is still required for native verification

Expected local file: `9) RE5DX9_1.1.0 Dev (Release 15 des. 2017).exe`,
19,977,216 bytes, SHA-256
`323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`.

Connected context searches returned prior-session sandbox references and
historical verification statements, but no accessible binary, usable attachment
ID or download location. The available working roots did not contain the file.
A historical hash statement is not a new hash calculation. Keep the EXE private
and local; never put it in this repository, a PR, or a GitHub artifact.

Pending work remains: materialize and hash that exact EXE; inspect the actual
00408700 body, boundaries, callers, guards and shared-base literal; compare or
correct this candidate; then register canonical source/tests/metadata, regenerate
reports, run the full validator, and carry out the requested separately observed
5/5-CI recovery, promotion and main publication. Do not infer an 18-byte native
body solely because the neighboring 86A0 constructor has that size.

## Scope safeguards

Only this provisional 00408700 directory and the already-existing 00408700
checkpoint belong to the continuation's delta. Canonical functions/claims,
history, denominators, root CMake, root README, reports, 8620, 86A0 and all
parallel implementations are unchanged relative to the synchronized integration
snapshot. No EXE, DLL, game asset, object file or build product is committed.
