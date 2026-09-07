# Agents

## Current mode

One coordinator only through the Pilot Final Gate.

Do not create permanent agent branches or guessed subsystem ownership during Pilot.

## Worker rules after scale is unlocked

A worker receives a narrow task packet containing:

- task ID;
- base commit;
- stable IDs/address range;
- claim owner;
- allowed scope;
- required transition;
- tests/evidence;
- branch name;
- PR target.

Workers must query canonical claims before work and may not silently broaden scope.

Scale begins with 2–4 workers only after `scale_allowed=true` is justified by a PASS Pilot Final Gate.
