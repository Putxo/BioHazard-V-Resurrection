# Branching

`main` is the public stable branch and is the **only branch whose README contains the progress dashboard, tier bars and treemap**.

`integration` is the technical integration branch. Its README must remain development-only and must not duplicate public progress percentages.

Work happens on short-lived task branches created from the current `integration` HEAD, for example:

- `pilot/<task>`
- `decomp/FUN_<VA>`
- `decomp/<range>`
- `refine/<stable-id-or-cluster>`
- `verify/<stable-id-or-cluster>`
- `inventory/<task>`
- `tools/<task>`

Rules:

1. Do not work directly on `main`.
2. Prefer PRs into `integration` for normal implementation work.
3. Publish validated milestones to `main` without replacing the public README with the development README.
4. Do not force-move historical branches merely to make their README match another branch. Merge/delete or retain them as history.
5. Do not create permanent branches per agent or guessed subsystem.
6. Delete ephemeral task branches after merge when they are no longer useful.
7. Never force-push shared branches except for an explicitly authorized recovery operation.
