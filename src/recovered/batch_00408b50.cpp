#include "re5/recovered/batch_00408b50.hpp"

namespace re5::recovered {
namespace {
using U32 = Batch408_U32;
constexpr U32 manager_slot = 0x0165A1F0U;
U32 read(Batch408_Access& a, U32 at) { return a.read32(a.context, at); }
void write(Batch408_Access& a, U32 at, U32 value) { a.write32(a.context, at, value); }
U32 call0(Batch408_Access& a, U32 target, U32 self) {
    return a.invoke(a.context, target, self, nullptr, 0U);
}
U32 call1(Batch408_Access& a, U32 target, U32 self, U32 argument) {
    const U32 args[]{argument};
    return a.invoke(a.context, target, self, args, 1U);
}
U32 call2(Batch408_Access& a, U32 target, U32 self, U32 first, U32 second) {
    const U32 args[]{first, second};
    return a.invoke(a.context, target, self, args, 2U);
}
U32 submit(Batch408_Access& a, U32 self, U32 selector) {
    return call2(a, 0x004B6EC0U, read(a, manager_slot), selector, self);
}
bool nonnegative(U32 value) { return (value & 0x80000000U) == 0U; }
void load_and_deliver(Batch408_Access& a, U32 resource_manager, U32 name,
                      U32 type, U32 receiver_slot, U32 delivery) {
    const U32 arguments[]{type, name, 1U};
    const U32 resource = a.invoke(a.context, 0x00D14C10U, resource_manager, arguments, 3U);
    if (resource != 0U) {
        call1(a, delivery, read(a, receiver_slot), resource);
        call0(a, 0x00D25060U, resource);
    }
}
} // namespace

Batch408_U32 FUN_00408B50(Batch408_Access& a, U32 self) {
    const U32 old = read(a, self + 0x34U);
    if (nonnegative(old)) {
        call2(a, 0x004B14A0U, read(a, manager_slot), old, self);
    }
    const U32 mode = call0(a, 0x004E6F10U, read(a, 0x0165BBB0U));
    const U32 selected = mode == 0U ? 0x9EU : 0x9DU;
    write(a, self + 0x34U, selected);
    return submit(a, self, selected);
}

bool FUN_00408B90(Batch408_Access& a, U32 self) {
    const U32 handle = read(a, self + 0x34U);
    if (!nonnegative(handle)) return true;
    const U32 status = call2(a, 0x004B1470U, read(a, manager_slot), handle, self);
    if (status == 2U) return true;
    if (status != 0U) return false;
    write(a, self + 0x34U, 0xFFFFFFFFU);
    return true;
}

void FUN_00408BC0(Batch408_Access& a, U32 self) {
    const U32 handle = read(a, self + 0x34U);
    if (nonnegative(handle)) {
        call2(a, 0x004B1430U, read(a, manager_slot), handle, self);
    }
}

void FUN_00408BE0(Batch408_Access& a, U32 self) {
    const U32 handle = read(a, self + 0x34U);
    if (!nonnegative(handle)) return;
    call2(a, 0x004B1430U, read(a, manager_slot), handle, self);
    // Both values are reloaded after the first call, including a now-negative
    // handle. There is no second validity guard in the original instructions.
    const U32 refreshed = read(a, self + 0x34U);
    call2(a, 0x004B14A0U, read(a, manager_slot), refreshed, self);
    write(a, self + 0x34U, 0xFFFFFFFFU);
}

Batch408_U32 FUN_00408C10(Batch408_Access& a, U32 self) { return submit(a, self, 0x98U); }
Batch408_U32 FUN_00408C30(Batch408_Access& a, U32 self) { return submit(a, self, 0x99U); }
Batch408_U32 FUN_00408C50(Batch408_Access& a, U32 self) { return submit(a, self, 0xA2U); }

Batch408_U32 FUN_00408C70(Batch408_Access& a, U32 self) {
    const U32 root = read(a, 0x016E1660U);
    const U32 mode = read(a, root + 0x1E4ECU);
    const U32 value = read(a, mode + 0x58U);
    return submit(a, self, value == 2U ? 0xA0U : 0xD1U);
}

Batch408_U32 FUN_00408CA0(Batch408_Access& a, U32 self) {
    const U32 root = read(a, 0x016E1660U);
    const U32 mode = read(a, root + 0x1E4ECU);
    const U32 value = read(a, mode + 0x58U);
    return submit(a, self, value == 3U ? 0xCDU : 0xCFU);
}

Batch408_U32 FUN_00408CD0(Batch408_Access& a, U32 self, U32 enabled) {
    return submit(a, self, (enabled & 0xFFU) == 0U ? 1U : 0U);
}

void FUN_00408CF0(Batch408_Access& a, U32 variant) {
    const U32 offset = variant == 0U ? 0U : 0x10U;
    // Retain each table/singleton load's position relative to external calls.
    U32 manager = read(a, 0x016E1D3CU);
    U32 name = read(a, 0x01568444U + offset);
    load_and_deliver(a, manager, name, 0x016E2DB8U, manager_slot, 0x004AFD20U);
    name = read(a, 0x01568448U + offset);
    manager = read(a, 0x016E1D3CU);
    load_and_deliver(a, manager, name, 0x01693688U, manager_slot, 0x004AFD40U);
    name = read(a, 0x0156844CU + offset);
    manager = read(a, 0x016E1D3CU);
    load_and_deliver(a, manager, name, 0x016E2830U, manager_slot, 0x004B13F0U);
    name = read(a, 0x01568450U + offset);
    manager = read(a, 0x016E1D3CU);
    load_and_deliver(a, manager, name, 0x016E22B0U, 0x016E2EFCU, 0x004C5060U);
}

void FUN_00408DD0(Batch408_Access& a) { FUN_00408CF0(a, 0U); }

Batch408_U32 FUN_00408DE0(Batch408_Access& a, U32 self) {
    call0(a, 0x00D02A20U, self);
    write(a, self + 0x28U, 0U);
    write(a, self + 0x30U, 0U);
    write(a, self, 0x0137E4A8U);
    return self;
}

void FUN_00408E00(Batch408_Access& a, U32 self) {
    write(a, self, 0x0137A990U);
    // KERNEL32!DeleteCriticalSection is stdcall: no this argument.
    call1(a, read(a, 0x0137119CU), 0U, self + 8U);
    write(a, self, 0x0137A97CU);
}
} // namespace re5::recovered
