#include "re5/recovered/fun_00408550.hpp"

namespace re5::recovered {
namespace {
const FUN_00408550_Services* g_services = nullptr;
}

void FUN_00408550_SetServices(
    const FUN_00408550_Services* services) noexcept {
    g_services = services;
}

FUN_00408550_Object* FUN_00408550(
    FUN_00408550_Object& object,
    const FUN_00408550_Object& source) noexcept {
    // Exact observable state/order of std::exception's copy constructor at
    // 0x01251B8B.
    object.vtable = FUN_00408550_EXCEPTION_VTABLE;
    object.base_owns_message = source.base_owns_message;

    if (source.base_owns_message == 0U) {
        object.base_message = source.base_message;
    } else if (source.base_message == 0U) {
        object.base_message = 0U;
    } else {
        const auto* services = g_services;

        // Host-only seam for the native strlen/allocate/copy sequence inside
        // std::exception's copy constructor. A null result matches the native
        // allocation-failure observable state.
        object.base_message =
            services != nullptr &&
                    services->duplicate_base_message != nullptr
                ? services->duplicate_base_message(
                      services->context,
                      object,
                      source.base_message)
                : 0U;
    }

    object.vtable = FUN_00408550_LOGIC_ERROR_VTABLE;

    // Native 8550 initializes only these visible SSO fields. The embedded
    // allocator_state dword at object+0x0C remains untouched.
    object.message.length = 0U;
    object.message.capacity = 0x0FU;
    reinterpret_cast<char*>(object.message.storage.data())[0] = '\0';

    (void)FUN_004082E0(
        object.message,
        source.message,
        0U,
        0xFFFFFFFFU);

    return &object;
}

} // namespace re5::recovered
