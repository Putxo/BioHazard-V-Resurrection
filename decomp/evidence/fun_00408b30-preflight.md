# FUN_00408B30 exclusive reservation and preflight

Preflight timestamp: `2026-09-25T07:51:13Z`.
Branch: `decomp/00408b30-next`; claim: `SEQ-00408B30`.

## Verified remote preflight

- main: `fb69da875dcfdd7efa90a1ab1fd5a1c160866d64`.
- integration: `b2a92926745ab1bcf3ffb006dea34a16516189ba`.
- Complete functions.csv and claims.csv parsed on both exact commits: no target stable ID or existing target VA.
- All fetched remote branch names checked case-insensitively: only the newly reserved target branch exists.
- All open PR pages checked; observed PRs: #124; no target collision.
- Repository metadata validator passed after appending only the exclusive claim.
- This PASS concerns preflight/reservation metadata only, NOT function implementation or CI 5/5.

Registry snapshots:

```json
{
  "fb69da875dcfdd7efa90a1ab1fd5a1c160866d64": {
    "database/functions.csv": {
      "blob": "a5be5671311d1e736a96d92bb012e31e5630f293",
      "records": 156
    },
    "database/claims.csv": {
      "blob": "c9d6561355db68b36bc948e36a32d5e95704046a",
      "records": 156
    }
  },
  "b2a92926745ab1bcf3ffb006dea34a16516189ba": {
    "database/functions.csv": {
      "blob": "55c295037346cdb2d9dc0b073238897a96d95e69",
      "records": 156
    },
    "database/claims.csv": {
      "blob": "faa5e423d3c087296c13bb57be423d268a3bd3f9",
      "records": 156
    }
  }
}
```

Parallel refs observed, never modified by this workflow:

```json
{
  "decomp/00408700-next": "40d27f09e7472d489d6450264c9120cf0b854c83",
  "decomp/00408af0-next": "7160a7bef46dfa4956241060f8928cc462133d8e"
}
```

## Assignment and evidence limits

- User-supplied candidate: row 144; RVA 35632 (0x00008B30); VA 4229936 (0x00408B30).
- Decimal/hex arithmetic and image-base relation verified. The local canonical function_candidates.csv is not mounted in this resumption environment; no independent CSV-row read is claimed.
- Expected private input: 9) RE5DX9_1.1.0 Dev (Release 15 des. 2017).exe; size 19,977,216 bytes.
- Expected input SHA-256: `323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`. This is an expected hash, not a freshly verified local EXE hash.
- The local analysis environment has no mounted EXE. Two Biblioteca/personal-context searches returned historical references but no binary bytes or usable current download reference.
- No executable is obtained from GitHub or uploaded to GitHub. This workflow reads repository text and metadata only.

## Current target status

**RESERVED; exact analysis blocked by missing private EXE bytes.**

No instruction bytes, native length, calling convention, dependencies, semantics, C++ implementation, unit-test result, FAST_PASS or FAST_PASS_VALIDATED result is asserted for this target.
No function row is added to functions.csv, so all recovery metrics and the canonical denominator of 79,782 remain unchanged.
No implementation PR, promotion or main publication has been performed. Do not merge this reservation as a completed recovery.

## Exact continuation point

Continue this branch and existing claim, not another function. Materialize the private EXE locally, verify its complete size/SHA-256, and verify the assigned row in the local canonical candidate CSV. Then analyze only 0x00408B30 and its necessary dependencies, reconstruct from actual instructions, add guard/early-exit tests and evidence, and complete the three independently checked CI/merge stages.
Preserve FUN_00408700, FUN_00408AF0, FUN_00408B10 and all concurrent work. Never upload EXE/DLL/assets/.obj or binary products.

Preflight execution: https://github.com/Putxo/BioHazard-V-Resurrection/actions/runs/36109768386
