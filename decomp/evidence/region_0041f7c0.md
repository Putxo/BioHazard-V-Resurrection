# Ordered decompilation evidence — 0x0041F7C0 through 0x0041FB00

- `0x0041F7C0`: clears owner three-entry pointer/value list `+0x618/+0x61C/+0x620` and count `+0x614`.
- `0x0041F7E0`: appends one value into first free of those three entries; increments count; if full logs error literal `0x0137EF00`.
- `0x0041F830`: returns literal/address `0x0137EF44`.
- `0x0041F840`: resets performance/stat sampling fields `+0x830..+0x854`, enables byte `+0x628`, mirrors each global timing node `0x016EF020[]` field `+0x04` into `+0x08`, clears 0x80-DWORD owner buffer `+0x630`, count `+0x62C`, lock byte `+0x629`.
- entry `0x0041F8F0`: if sampling enabled, computes frame/timing samples from root timing and `0x00CF67E0`, updates running averages `+0x838/+0x83C/+0x84C`, elapsed sample `+0x850`, maxima `+0x840/+0x844`.
- `0x0041FA40`: converts global 64-bit-ish timing fields `root+0x20/+0x24` to scaled float via constant `0x0137EF4C`, stores owner `+0x830`.
- `0x0041FA90`: when enabled clears sample/max fields and stores similarly converted timing value into `+0x834`.
- `0x0041FB00`: thread/spin guarded append of non-null pointer into owner array `+0x630`, capacity 0x80, avoiding duplicates; lock byte `+0x629`, count `+0x62C`.
- entry `0x0041FB60`: resolves three named resources via `0x00489ED0` into owner `+0x858/+0x85C/+0x860`, then uses owner ID `+0x2C` jump table to select root resource transition and set root state `+0x1E8C8`; body continues to boundary `0x0041FEE0`.

No VERIFIED/MATCHED claim.