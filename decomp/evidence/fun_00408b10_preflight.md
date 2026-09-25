# FUN_00408B10 exclusive reservation and preflight

Date: 2026-09-25.
Scope: FUN_00408B10 only.
Reserved branch: `decomp/00408b10-next`.
Status: branch reserved; exact local binary input is not available in this session.
This is a coordination record, not native-code evidence or a completed recovery.

## Assigned candidate

The user supplied the following canonical candidate identity:

- VA decimal: `4229904` = `0x00408B10`.
- RVA decimal: `35600` = `0x00008B10`.
- Image base: `0x00400000`.
- Exact row number supplied by the user: `143` in `function_candidates.csv`.

The decimal/hexadecimal arithmetic was checked. The canonical local CSV is
not present in this session, so its row contents and candidate membership
have not been independently re-read here. No false-positive classification
or denominator reduction is justified or made.

## Remote preflight

Initial snapshots:

- main: `9f9af9693fb14baa0a1505b98bbbe731e4b329b7`.
- integration: `b2a92926745ab1bcf3ffb006dea34a16516189ba`.

The complete `database/functions.csv` and `database/claims.csv` from both
snapshots were read and searched for the exact target identifier. Neither
contained FUN_00408B10. Branch search for `00408b10` returned no branch;
repository PR search for `00408B10` returned zero results.

Before branch creation, integration was re-read and still pointed to the
same commit. Main had advanced to
`fb69da875dcfdd7efa90a1ab1fd5a1c160866d64`. The comparison against the initial
main snapshot changed only unrelated verification/code/test paths, not the
functions or claims registries. A repeated branch search still returned
no `00408b10` branch. The new exclusive branch was created from the exact
integration SHA above, without force-updating any existing ref.

## Required local input, not yet verified from bytes

Expected executable:
`9) RE5DX9_1.1.0 Dev (Release 15 des. 2017).exe`.

Expected size from the canonical inventory: `19977216` bytes.
Expected SHA-256 from the canonical inventory:
`323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`.

The mounted input directory is empty. Searches of the local input/work
locations and the connected historical context found references to prior
attachments, but no materializable binary or accessible attachment reference
for this session. The historical sandbox path is not an available file here.
These expected size/hash values are not represented as a newly computed
hash of an executable.

No EXE was fetched from GitHub or sent to GitHub Actions. No native bytes,
function size, dependencies, guards, calling convention, or behavior are
inferred from adjacent functions.

## Preservation and continuation

This reservation adds only this text record. It does not change canonical
functions, claims, history, reports, README, build wiring, or source/tests.
It does not mark FAST_PASS or FAST_PASS_VALIDATED, and it does not claim any
new implementation/test/CI success. No implementation, promotion, or
publication PR has been opened for this reservation.

FUN_00408700, FUN_00408AF0 and all parallel branches remain outside this task.
The canonical denominator stays `79782`; inventory freeze is unchanged.
No executable, DLL, proprietary asset, object file, or build product is added.

Resume this same branch after making the exact executable available as a
current local attachment. First repeat remote preflight, verify the actual
file size/SHA-256 and re-read candidate membership when the local CSV is
available. Then perform local-only native analysis, original implementation,
target-specific tests, evidence and metadata/report validation. Integration
recovery, FAST_PASS_VALIDATED promotion, and incremental main publication
must each satisfy the requested five actual CI checks before their merges.
