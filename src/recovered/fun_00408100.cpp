#include "re5/recovered/fun_00408100.hpp"

#include "re5/recovered/fun_00407e70.hpp"

#include <cstring>

namespace re5::recovered {
namespace {
const FUN_00408100_Services* g_services = nullptr;

static_assert(sizeof(char*) <= 0x10U);

char* load_heap_pointer(FUN_00408100_Object& object) noexcept {
    char* pointer = nullptr;
    std::memcpy(&pointer, object.storage.data(), sizeof(pointer));
    return pointer;
}

void store_heap_pointer(
    FUN_00408100_Object& object,
    char* pointer) noexcept {
    std::memcpy(object.storage.data(), &pointer, sizeof(pointer));
}

char* data_pointer(FUN_00408100_Object& object) noexcept {
    if (object.capacity < FUN_00408100_SSO_THRESHOLD) {
        return reinterpret_cast<char*>(object.storage.data());
    }
    return load_heap_pointer(object);
}

void reset_after_failed_exact_retry(FUN_00408100_Object& object) noexcept {
    if (object.capacity >= FUN_00408100_SSO_THRESHOLD) {
        const auto* services = g_services;
        if (services != nullptr && services->free_aligned != nullptr) {
            services->free_aligned(
                services->context,
                load_heap_pointer(object));
        }
    }

    object.capacity = FUN_00408100_INLINE_CAPACITY;
    object.size = 0U;
    object.storage[0] = std::byte{0};
}

} // namespace

void FUN_00408100_SetServices(
    const FUN_00408100_Services* services) noexcept {
    g_services = services;
}

void FUN_00408100(
    FUN_00408100_Object& object,
    std::uint32_t requested_capacity,
    std::uint32_t preserved_size) {
    const auto* services = g_services;

    // Host-only guard: the native target's aligned release routine is always
    // present. Refuse a heap-to-heap transition when the host shim needed to
    // release the old allocation is unavailable.
    if (object.capacity >= FUN_00408100_SSO_THRESHOLD &&
        (services == nullptr || services->free_aligned == nullptr)) {
        return;
    }

    std::uint32_t new_capacity =
        requested_capacity | FUN_00408100_INLINE_CAPACITY;
    if (new_capacity <= 0xFFFFFFFEU) {
        const std::uint32_t old_capacity = object.capacity;
        const std::uint32_t half_capacity = old_capacity >> 1U;

        if ((new_capacity / 3U) < half_capacity &&
            old_capacity <= (0xFFFFFFFEU - half_capacity)) {
            new_capacity = old_capacity + half_capacity;
        }
    } else {
        new_capacity = requested_capacity;
    }

    FUN_00407E70_Object ignored_allocator_object{};
    char* replacement = nullptr;

    try {
        replacement = static_cast<char*>(
            FUN_00407E70(
                ignored_allocator_object,
                new_capacity + 1U));

        // Host-only guard inherited from the portable E70/A90 service path.
        if (replacement == nullptr) {
            return;
        }
    } catch (...) {
        // Native first catch-all retries with exact requested_capacity.
        new_capacity = requested_capacity;
        try {
            replacement = static_cast<char*>(
                FUN_00407E70(
                    ignored_allocator_object,
                    new_capacity + 1U));

            if (replacement == nullptr) {
                return;
            }
        } catch (...) {
            // Native second catch-all resets to empty SSO and rethrows.
            reset_after_failed_exact_retry(object);
            throw;
        }
    }

    char* const old_data = data_pointer(object);

    if (preserved_size > 0U) {
        std::memcpy(replacement, old_data, preserved_size);
    }

    if (object.capacity >= FUN_00408100_SSO_THRESHOLD) {
        services->free_aligned(services->context, old_data);
    }

    object.storage[0] = std::byte{0};
    store_heap_pointer(object, replacement);
    object.capacity = new_capacity;
    object.size = preserved_size;

    char* const installed_data =
        new_capacity < FUN_00408100_SSO_THRESHOLD
            ? reinterpret_cast<char*>(object.storage.data())
            : replacement;
    installed_data[preserved_size] = '\0';
}

} // namespace re5::recovered
