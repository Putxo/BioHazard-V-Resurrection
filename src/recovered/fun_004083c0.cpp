#include "re5/recovered/fun_004083c0.hpp"

#include <cstdint>
#include <cstring>
#include <stdexcept>

namespace re5::recovered {
namespace {
const FUN_004083C0_Services* g_services = nullptr;

char* string_data(FUN_004082E0_String& value) noexcept {
    if (value.capacity < 0x10U) {
        return reinterpret_cast<char*>(value.storage.data());
    }

    char* pointer = nullptr;
    static_assert(sizeof(pointer) <= 0x10U);
    std::memcpy(&pointer, value.storage.data(), sizeof(pointer));
    return pointer;
}
} // namespace

void FUN_004083C0_SetServices(
    const FUN_004083C0_Services* services) noexcept {
    g_services = services;
}

FUN_004082E0_String* FUN_004083C0(
    FUN_004082E0_String& destination,
    const char* source,
    std::uint32_t count) {
    char* const current_data = string_data(destination);
    const std::uintptr_t begin =
        reinterpret_cast<std::uintptr_t>(current_data);
    const std::uintptr_t source_address =
        reinterpret_cast<std::uintptr_t>(source);

    // Native alias test is [data, data + length). Use integer addresses so
    // the portable host does not rely on relational comparison of unrelated
    // C++ pointers.
    if (source_address >= begin &&
        source_address - begin < destination.length) {
        const std::uint32_t position =
            static_cast<std::uint32_t>(source_address - begin);
        return FUN_004082E0(
            destination,
            destination,
            position,
            count);
    }

    if (count > 0xFFFFFFFEU) {
        throw std::length_error("string too long");
    }

    if (destination.capacity < count) {
        const auto* services = g_services;

        // Host-only guard. Native code calls FUN_00408100 directly and
        // assumes the growth helper either succeeds or throws.
        if (services == nullptr ||
            services->grow == nullptr ||
            !services->grow(
                services->context,
                destination,
                count,
                destination.length) ||
            destination.capacity < count) {
            return &destination;
        }
    }

    // Native zero-length path does not dereference source.
    if (count == 0U) {
        destination.length = 0U;
        string_data(destination)[0] = '\0';
        return &destination;
    }

    char* const destination_data = string_data(destination);
    std::memmove(destination_data, source, count);
    destination.length = count;
    destination_data[count] = '\0';
    return &destination;
}

} // namespace re5::recovered
