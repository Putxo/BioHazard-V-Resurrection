# Coordination

The repository is currently in **Phase 14 — Pilot Fast Pass** with one coordinator. Multi-agent work remains locked.

Before each task:

1. fetch the current `integration` state;
2. inspect open PRs, claims and blockers;
3. create a short-lived task branch;
4. claim exact stable IDs/ranges;
5. analyze only the claimed scope;
6. implement real C/C++;
7. build/test/validate metadata;
8. update evidence and state history;
9. open a PR into `integration`;
10. merge only after real CI success.

For sequential decompilation, prefer ascending VA within the active pilot/assigned region unless dependency evidence requires a different order. Do not skip ahead to mass parallelization before the Pilot Final Gate.

`main` alone publishes the public dashboard. `integration` and work branches keep development READMEs without public progress bars.
