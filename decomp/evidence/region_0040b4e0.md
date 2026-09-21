# Ordered decompilation evidence — 0x0040B4E0 through 0x0040B970

## FUN_0040B4E0

- No stack args; plain `ret`.
- Chooses between helper `0x00AC11D0` for global mode `+0x58 == 1` and helper `0x00AC1240` otherwise.
- The boolean result selects an effect/message id. A random source `0x0119F610(context 0x016F0230)` supplies the low bit for IDs `0x34/0x35`; alternate path uses `0x36`.
- Dispatches via `0x005122D0(global 0x0165C170, 0x0C, mode==5 ? 0x0E : 0x0D, selected_id, float[0x0137E510], -1, 0)`.

## FUN_0040B560

- Reads global state `+0x5DC`, maps it through `0x00AC13B0`, then uses the result as a four-slot stride (`index << 6`) into state at `+0x6B8`.
- Normalizes the selected slot state differently for global mode `1` vs other modes, using `0x00AC11D0`/`0x00AC1240`.
- Five normalized cases choose randomized ID groups using `0x0119F610`: `0x41..0x43`, `0x44..0x45`, `0x46..0x48`, `0x49..0x4B`, `0x4C..0x4D`.
- Valid selections are dispatched through the same `0x005122D0` service and mode-dependent `0x0D/0x0E` selector used by neighboring routines.
- Conservative interpretation: randomized feedback/effect selector driven by current slot state.

## FUN_0040B6B0

- `thiscall`, one stack float argument, `ret 0x04`; large state-update routine ending at `0x0040B914`.
- Early exits unless global state fields `+0x574` and `+0x578` match.
- Operates primarily on owner state `this+0x4D4`, timer `+0x4DC`, mask `+0x4D8`, accumulator `+0x4E0`.
- State `0`: decrements timer by delta unless global flag `0x2000` is set; clamps at zero and advances owner state to `1` when expired.
- Mirrors timer into global state `+0x67C` when byte `+0x56C` is clear.
- For global modes `2/5`, threshold logic based on byte `+0x79A` and DWORD `+0x58C` can force state transition.
- Reads input/status through global `0x016E2FD0` using helpers `0x004AA600` and `0x004AA5A0`; masks `0xC00/0x400` can force transition and zero the timer.
- On transition, modes `2/5` derive a randomized integer and store it to global state `+0x68C`.
- Calls global service `0x016E03CC`: `0x004A2190` for state/timer transition and `0x004A0100(...,0x19,integer_timer)` periodically.
- State `1` scans four global slot values rooted at `+0x630`; owner mask `+0x4D8` controls pending slots. Completion changes owner state to `2` and sends a zero-valued transition.
- Conservative interpretation: multi-stage timed state machine synchronized with global four-slot/input state.

## FUN_0040B920

- `stdcall`, one float delta, `ret 0x04`.
- Unless global flag `0x2000` is set or global state `+0x678 == 3`, subtracts delta from global float `+0x67C` and clamps to zero.
- Pure global timer update.

## FUN_0040B970

- Starts by reading global mode `+0x58` and immediately rejects modes `0`, `3`, and `4`.
- Remaining body continues beyond the excerpt used for this evidence pass; function is not marked complete here.

Next ordered work remains inside `FUN_0040B970`, then candidate `0x0040B9E0`. No VERIFIED/MATCHED claim.