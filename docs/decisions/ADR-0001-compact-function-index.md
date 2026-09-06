# ADR-0001 — Compact immutable function index + sparse semantic overlay

## Context

The Phase 11 executable contains tens of thousands of function candidates. A fully expanded CSV repeats VA/RVA/default tier fields for every row, creates a multi-megabyte merge hotspot and makes future multi-agent work unnecessarily conflict-prone.

## Decision

The canonical `database/` remains the single source of truth, but function information is split by responsibility:

1. `database/function_index/` is a deterministic compact binary-derived inventory: stable VA, estimated size and detection evidence/confidence.
2. `database/functions.csv` is a sparse semantic/tier overlay keyed by stable ID and contains only functions whose human/recovery metadata differs from inventory defaults.
3. Tooling materializes a merged function view for reports, claims and validation.

The compact index is immutable for a given inventory revision. Corrections are regenerated deterministically and recorded through Git/history rather than hand-editing individual encoded records.

## Why this is safer

- preserves one canonical Git-versioned database;
- preserves stable VA identity;
- avoids tens of thousands of duplicate default fields;
- reduces merge conflicts for later agents;
- makes inventory reproduction possible from the exact local target;
- lets semantic/tier progress change without rewriting the entire binary-derived index.

## Constraint

No function is considered confirmed merely because it exists in the candidate index. Confidence and evidence are explicit, and Phase 12/13 may correct the candidate set before Inventory Freeze.
