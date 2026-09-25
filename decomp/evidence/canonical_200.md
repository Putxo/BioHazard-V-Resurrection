# Canonical registration milestone: 156 to 200

## What changes

This milestone publishes fourteen already-integrated wrappers (`00408B50` through
`00408E00`) and registers thirty already-published field-reader implementations
from `decomp/leaf323d1aab`. They add **44 unique registry entries relative to main**,
not 44 newly generated or duplicate source bodies. Main's preflight registry has
156 entries; integration has 170. The resulting registry has **200 / 79,782**.

The existing dashboard measures registered/discovered functions, not fully linked
or byte-identical functions. The pre-existing DISASSEMBLED entry remains unchanged.
No CONVERTED, REFINED, VERIFIED, MATCHED or LINKED flags are raised by this milestone.
FAST_PASS_VALIDATED is promoted only after the recovery PR's real CI is checked.

## Exact input and membership

The sole local input is the 19,977,216-byte PE32 with SHA-256
`323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`.
It is neither uploaded nor launched. All 44 body ranges and direct-call references
were checked against that exact attachment again.

The V6 reconstruction reproduces 19,194 direct CALL targets (19,195 roots with the
PE entry), 40,912 pointer-sequence targets and 69,076 post-padding starts. Their
union has 79,782 unique addresses. Crucially, it reproduces the **entire sorted-set
SHA-256 already committed in `batch_00408b50.json`**, not just the aggregate count:

`a7fc3ad913d379f488b8de0a097a6cad290c41fe3cee921f39e3512dee3a4a25`

All eight recorded direct-group line anchors match. A new, explicitly labelled
CSV serialization of this reproduced set is materialized locally and its decimal
VA column is read back to check the 44 exact members. Per-entry CSV line numbers,
body hashes, call sites and existing implementation paths are in
[`canonical_200.json`](canonical_200.json). The historical CSV file's original
bytes were **not** available; this is not represented as a byte-for-byte recovery
of that file. The old frozen denominator and all historical exclusions are left
untouched. No candidate outside the reproduced, previously committed universe is
registered. This makes the membership method reproducible instead of relying on
an unsupported sum of earlier batch counts.

## Revalidation

The thirty leaves have 162 original instruction bytes: exact-width unsigned
field reads, with no invented null guard. The focused regression suite invokes
the **existing source definitions**, rather than substitutes. GCC and Clang with
ASan/UBSan each passed 491,521 property checks and 240 protected-memory checks.
Every entry is checked under eight receiver alignments and all low-byte values;
the boundary tests permit only the accessed page and make it read-only. All thirty
selected Windows I386 definitions compiled and their body bytes matched the
recorded originals using local Clang. That limited comparison does not trigger a
canonical MATCHED promotion.

The fourteen wrappers have 632 original instruction bytes. Their existing local
bounded-instruction differential test passed again: 2,371 scenarios, 9,597 ordered
memory/call events and all 26 conditional branch edges. Their external callees
remain explicit fixtures, not implementations or guessed successful no-ops. See
[`batch_00408b50.md`](batch_00408b50.md) for the scope of that adapter.

The dedicated CI also executes the thirty leaf implementations and original-hash
instruction fixtures in native I386 mode with GCC and Clang, requiring 491,761
checks for each set. The fragments have no external calls; the original PE is
never loaded. Native CI results must be read separately from local compilation.
The full existing repository build, all module workflows, metadata, claims and
progress checks remain enabled. The local full GCC Release suite passed 33/33
CTest groups after retaining assertions in the legacy smoke test target. Its
fixtures contain setup inside assert(); NDEBUG previously removed that setup and
the Release smoke test faulted. The fix adds -UNDEBUG (or /UNDEBUG) only to the
test target, never to recovered library code, and restores checks rather than
disabling a failing test. The verification workflow has read-only access.

## Reproduce without changing the baseline

```sh
python tools/verification/verify_canonical_200.py --registry
python tools/verification/verify_canonical_200.py \
  --exe /local/path/to/exact-attachment.exe \
  --csv-output /local/output/function_candidates_reproduced.csv
python tools/verification/verify_canonical_200.py \
  --native --compiler g++ --build-dir /tmp/canonical-200-i386
```

Use an external build/output directory; no executable, DLL, object, original asset
or generated instruction fixture is committed. `tools/maintenance/canonical_200.py`
appends or promotes only the explicit target IDs and publishes incrementally from
main, preserving main-only files and all unrelated rows. It refuses changed base
counts instead of silently overwriting parallel progress. The temporary source
export/preparation workflows are not part of the published change.
