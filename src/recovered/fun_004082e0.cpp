#include "re5/recovered/fun_004082e0.hpp"

#include <cstring>
#include <stdexcept>

namespace re5::recovered {
namespace {
const FUN_004082E0_Services* g_services = nullptr;

char* string_data(FUN_004082E0_String& value) noexcept {
    if (value.capacity < 0x10U) {
        return reinterpret_cast<char*>(value.storage.data());
    }

    char* pointer = nullptr;
    static_assert(sizeof(pointer) <= 0x10U);
    std::memcpy(&pointer, value.storage.data(), sizeof(pointer));
    return pointer;
}

const char* string_data(const FUN_004082E0_String& value) noexcept {
    if (value.capacity < 0x10U) {
        return reinterpret_cast<const char*>(value.storage.data());
    }

    const char* pointer = nullptr;
    static_assert(sizeof(pointer) <= 0x10U);
    std::memcpy(&pointer, value.storage.data(), sizeof(pointer));
    return pointer;
}

void erase_range(
    FUN_004082E0_String& value,
    std::uint32_t position,
    std::uint32_t count) noexcept {
    const std::uint32_t remaining = value.length - position;
    const std::uint32_t erase_count =
        count < remaining ? count : remaining;
    if (erase_count == 0U) {
        return;
    }

    char* data = string_data(value);
    const std::uint32_t tail = remaining - erase_count;
    std::memmove(
        data + position,
        data + position + erase_count,
        tail);
    value.length -= erase_count;
    data[value.length] = '\0';
}
} // namespace

void FUN_004082E0_SetServices(
    const FUN_004082E0_Services* services) noexcept {
    g_services = services;
}

FUN_004082E0_String* FUN_004082E0(
    FUN_004082E0_String& destination,
    const FUN_004082E0_String& source,
    std::uint32_t position,
    std::uint32_t count) {
    if (position > source.length) {
        throw std::out_of_range("invalid string position");
    }

    const std::uint32_t available = source.length - position;
    const std::uint32_t copy_count =
        count < available ? count : available;

    // Native self-source branch delegates twice to FUN_00408070. Reproduce
    // the already-proven erase contract locally so this recovery remains
    // link-independent from parallel helper publication.
    if (&destination == &source) {
        erase_range(
            destination,
            position + copy_count,
            0xFFFFFFFFU);
        erase_range(destination, 0U, position);
        return &destination;
    }

    if (copy_count > 0xFFFFFFFEU) {
        throw std::length_error("string too long");
    }

    if (destination.capacity < copy_count) {
        const auto* services = g_services;

        // Host-only guard for the native FUN_00408100 growth dependency.
        // The target assumes that helper is present and succeeds.
        if (services == nullptr ||
            services->grow == nullptr ||
            !services->grow(
                services->context,
                destination,
                copy_count,
                destination.length)) {
            return &destination;
        }
    }

    // Native zero-length path never resolves source storage.
    if (copy_count == 0U) {
        destination.length = 0U;
        string_data(destination)[0] = '\0';
        return &destination;
    }

    const char* source_data = string_data(source);
    char* destination_data = string_data(destination);
    std::memmove(
        destination_data,
        source_data + position,
        copy_count);
    destination.length = copy_count;
    destination_data[copy_count] = '\0';
    return &destination;
}

} // namespace re5::recovered
