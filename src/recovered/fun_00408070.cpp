#include "re5/recovered/fun_00408070.hpp"

#include <cstring>
#include <stdexcept>

namespace re5::recovered {
namespace {

char* string_data(FUN_00408070_String& value) noexcept {
    if (value.capacity < 0x10U) {
        return reinterpret_cast<char*>(value.storage.data());
    }

    // The native x86 object stores a 32-bit heap pointer in the first dword of
    // the 16-byte storage union. The host harness keeps its native-sized
    // pointer in the same union without changing any observed object offsets.
    char* pointer = nullptr;
    static_assert(sizeof(pointer) <= 0x10U);
    std::memcpy(&pointer, value.storage.data(), sizeof(pointer));
    return pointer;
}

} // namespace

FUN_00408070_String* FUN_00408070(
    FUN_00408070_String& value,
    std::uint32_t position,
    std::uint32_t count) {
    if (position > value.length) {
        throw std::out_of_range("invalid string position");
    }

    const std::uint32_t remaining = value.length - position;
    const std::uint32_t erase_count =
        count < remaining ? count : remaining;

    // Native early return: no storage pointer is read when nothing is erased.
    if (erase_count == 0U) {
        return &value;
    }

    char* data = string_data(value);
    const std::uint32_t tail = remaining - erase_count;

    std::memmove(
        data + position,
        data + position + erase_count,
        tail);

    value.length -= erase_count;
    data[value.length] = '\0';
    return &value;
}

} // namespace re5::recovered
