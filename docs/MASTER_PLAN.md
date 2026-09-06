# Master Plan

This repository follows a strict gated sequence. No phase may start before the previous phase has a PASS gate.

## Required sequence

0. Bootstrap
1. Repository Foundation
2. Target Definition
3. Canonical Database + Schemas
4. Build System
5. Validation Core
6. Claims / Ownership
7. Reporting + README + Treemap
8. CI
9. Foundation Freeze
10. Binary Inventory
11. Function Inventory
12. Xref / Global / RTTI / Vtable Inventory
13. Inventory Freeze
14. Pilot Fast Pass
15. Pilot Conversion
16. Pilot Refine
17. Pilot Verified
18. Matching Feasibility
19. Linkage / Integration Model
20. Pilot Final Gate
21. Controlled Scale
22. Multi-Agent Operation
23. Long-Run Coverage
24. Final Refinement
25. Completion

## Non-negotiable rules

- The binary target and proprietary assets stay local and are never committed.
- The canonical database is the single source of truth once Phase 3 is complete.
- Stable IDs retain the original VA identity.
- No fake function counts, names, modules, statuses, tests, matches, or progress.
- C++ is the primary implementation language; C may be used for naturally procedural units.
- Work begins with one coordinator through Foundation, Inventory, and Pilot.
- Claims become mandatory before parallel function work.
- `main` and `integration` are never direct work branches after bootstrap.
- Every phase has a documented gate and must be 100% complete before the next phase.
