#pragma once

namespace re5::recovered {
using Batch408_U32 = unsigned int;
static_assert(sizeof(Batch408_U32) == 4, "Target words must be 32-bit");

// Explicit boundary to target memory and not-yet-recovered callees. Addresses
// are PE32 tokens, never host pointers. All three callbacks are required.
// A callback must report invalid memory/calls; silently inventing values is not
// an implementation of a dependency. This adapter is not the final native ABI.
struct Batch408_Access {
    void* context;
    Batch408_U32 (*read32)(void*, Batch408_U32 address);
    void (*write32)(void*, Batch408_U32 address, Batch408_U32 value);
    Batch408_U32 (*invoke)(void*, Batch408_U32 target, Batch408_U32 this_address,
                          const Batch408_U32* arguments, Batch408_U32 count);
};

Batch408_U32 FUN_00408B50(Batch408_Access&, Batch408_U32 self);
bool FUN_00408B90(Batch408_Access&, Batch408_U32 self);
void FUN_00408BC0(Batch408_Access&, Batch408_U32 self);
void FUN_00408BE0(Batch408_Access&, Batch408_U32 self);
Batch408_U32 FUN_00408C10(Batch408_Access&, Batch408_U32 self);
Batch408_U32 FUN_00408C30(Batch408_Access&, Batch408_U32 self);
Batch408_U32 FUN_00408C50(Batch408_Access&, Batch408_U32 self);
Batch408_U32 FUN_00408C70(Batch408_Access&, Batch408_U32 self);
Batch408_U32 FUN_00408CA0(Batch408_Access&, Batch408_U32 self);
Batch408_U32 FUN_00408CD0(Batch408_Access&, Batch408_U32 self, Batch408_U32 enabled);
void FUN_00408CF0(Batch408_Access&, Batch408_U32 variant);
void FUN_00408DD0(Batch408_Access&);
Batch408_U32 FUN_00408DE0(Batch408_Access&, Batch408_U32 self);
void FUN_00408E00(Batch408_Access&, Batch408_U32 self);
} // namespace re5::recovered
