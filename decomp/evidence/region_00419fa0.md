# Ordered decompilation evidence — 0x00419FA0 through 0x0041A8C0

## FUN_00419FA0
- `thiscall`; exceptionally large high-level state machine, owner byte `+0x05` range up to `0x5B`, dispatch tables `0x0041A838/0x0041A860`.
- Initial path requests root flags `0x40000000/0x20000000`, clears `0x80000/0x10000`, resets UI, disables one flag group, determines a preferred/valid player from owner/global/current-user state and stores owner `+0x120/+0x124`.
- Resets global player/service selectors for four players; root `0x08000000` path performs player/object eligibility checks and may invoke player action codes.
- Contains special current-user progression gates (IDs/values such as `0x2C1`, `0x72`, `0x528`) and arrays of action codes (`0x43,0x44,0x45,0x46,0x37,0x41,0x38,0x39`).
- Synchronously sets root flag groups `0x00400000`, `0x01000000`, `0x00800000`; enables player manager, requests `0x8000`, dispatches global code `0x97`, updates active-player records.
- Reuses owner object `+0x11C` ownership lifecycle and numerous later state-machine branches (UI/service readiness, global selectors, timers, player scans, mode transitions). Body reaches dispatch-table tail around `0x0041A81E`.
- Strong semantics: top-level multi-stage gameplay/session transition coordinator; exact symbolic states require refinement.

Next ordered candidate `0x0041A8C0`. No VERIFIED/MATCHED claim.