# Ordered decompilation evidence — 0x0041DAD0 through 0x0041DF30

## FUN_0041DAD0
- `stdcall`, index arg. Computes 0x50-byte per-index record in current-user structure at `+0x23310`; returns null if inactive byte zero or if record `+0x08` equals caller index, otherwise returns record pointer.

## FUN_0041DB20
- `thiscall`; resolves three literal/token resources (`0x0137ECB4`, `0x0137EC9C`, `0x0137EC80`) and when present configures global service slots 0/1/2 through `0x0050BA80` plus external loaders `0x005E9EE0/0x005EA0A0/0x005EA230` using owner `+0x2C`; finally passes virtual slot `+0x10` result to `0x0050B6F0`.

## FUN_0041DBD0
- `thiscall`; large one-shot resource/configuration setup guarded by owner byte `+0x28`.
- Enumerates global collection `0x0165BC60`, resolves descriptor IDs into resources via `0x01199AB0`, registers them with service `0x0165C148`.
- Switches on owner ID `+0x2C` with many exact IDs (`0x69,0x6C,0x6D,0x6F,0x71,0xC8..0xD1,0x12C,0x131,0x133,0x1F4,0x1F9,0x1FA,0x2BE/0x2BF`, etc.) and emits corresponding service commands through `0x0050BDA0` with fixed selector/value combinations.
- Finishes via service `0x0050B020`; strong semantics: ID-specific resource/command preload table encoded as branches.

Next ordered candidate `0x0041DF30`. No VERIFIED/MATCHED claim.