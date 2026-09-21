# Ordered candidate map: 0x00424EA0 - 0x004265A0

Dual-purpose continuation record: boundaries were recovered from the verified target executable using compiler INT3 padding plus direct linear decoding. This does **not** mark candidates VERIFIED/MATCHED.

Padded candidate starts encountered in strict address order:

`00424EA0`, `004254F0`, `00425500`, `00425710`, `00425C70`, `00425C80`, `00425CD0`, `00425E00`, `00425E10`, `00425E40`, `00425E50`, `00425EE0`, `00425F00`, `00426030`, `00426060`, `00426070`, `004260A0`, `004260C0`, `004260D0`, `004260E0`, `004260F0`, `00426100`, `00426110`, `00426120`, `00426130`, `00426140`, `00426170`, `004264B0`, `004264C0`, `004264D0`, `00426500`, `00426510`, `00426530`, `00426540`, `00426550`, `00426560`, `00426570`, `00426580`, `00426590`, `004265A0`.

Notable cross-reference already established from the preceding recovered region: `FUN_00422BD0` inserts/binds derived objects into owner container `+0x5F4` through `0x00425CD0`, so `FUN_00425CD0` belongs to the local container/registration machinery rather than being an unrelated entrypoint.

The unusually dense run of 0x10-spaced entries from `0x00426060` onward is retained exactly as separate padded candidates. They must not be merged or given semantic names merely because they are short; later passes should classify their accessor/thunk behavior from exact instructions and XREFs.
