#include "re5/recovered/fun_004084d0.hpp"

#include <cstring>

namespace re5::recovered {
namespace {
const FUN_004084D0_Services* g_services = nullptr;

static_assert(sizeof(char*) <= 0x10U);

char* load_heap_pointer(
    FUN_004082E0_String& value) noexcept {
    char* pointer = nullptr;
    std::memcpy(
        &pointer,
        value.storage.data(),
        sizeof(pointer));
    return pointer;
}
} // namespace

void FUN_004084D0_SetServices(
    const FUN_004084D0_Services* services) noexcept {
    g_services = services;
}

void FUN_004084D0(
    FUN_004084D0_Object& object) noexcept {
    const auto* services = g_services;

    // Native std::logic_error destructor body first restores the
    // std::logic_error vtable before destroying its embedded SSO message.
    object.vtable =
        FUN_004084D0_LOGIC_ERROR_VTABLE;

    if (object.message.capacity >= 0x10U) {
        // Host-only guard. Target aligned free 0x0125145C is always present.
        if (services != nullptr &&
            services->free_aligned != nullptr) {
            services->free_aligned(
                services->context,
                load_heap_pointer(object.message));
        }
    }

    object.message.capacity = 0x0FU;
    object.message.length = 0U;
    object.message.storage[0] = std::byte{0};

    // Exact tail target 0x01251BE3 is std::exception::~exception().
    object.vtable =
        FUN_004084D0_EXCEPTION_VTABLE;

    if (object.base_owns_message != 0U) {
        // Host-only guard for the opaque target base-message release routine
        // at 0x0125677A. Native code has no missing-service path.
        if (services != nullptr &&
            services->release_base_message != nullptr) {
            services->release_base_message(
                services->context,
                object.base_message);
        }
    }
}

} // namespace re5::recovered
