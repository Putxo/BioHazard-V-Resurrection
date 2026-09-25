#include "re5/recovered/fun_004022f0.hpp"

#include <cstring>

namespace re5::recovered {
namespace {
const FUN_00402360_Services* g_services = nullptr;
}

void FUN_00402360_SetServices(const FUN_00402360_Services* services) noexcept {
    g_services = services;
}

int FUN_004022F0(const FUN_004022F0_Object* object, const char* input) noexcept {
    if (input == nullptr) {
        return 0;
    }

    if (object != nullptr) {
        const char* stored = object->string_at_08;
        if (stored == nullptr) {
            return 0;
        }
        // Exact attached target, 00402300..0040233E: compare INPUT against
        // STORED as unsigned bytes; SBB/SBB normalizes to -1, 0 or +1.
        // Keep both early NUL exits instead of delegating reads to libc.
        const auto* argument = reinterpret_cast<const unsigned char*>(input);
        const auto* text = reinterpret_cast<const unsigned char*>(stored);
        for (;;) {
            const unsigned char first = argument[0];
            if (first != text[0]) {
                return first < text[0] ? -1 : 1;
            }
            if (first == 0U) {
                return 0;
            }
            const unsigned char second = argument[1];
            if (second != text[1]) {
                return second < text[1] ? -1 : 1;
            }
            argument += 2;
            text += 2;
            if (second == 0U) {
                return 0;
            }
        }
    }

    const auto* services = g_services;
    const unsigned char fallback = services != nullptr && services->fallback_literal != nullptr
        ? static_cast<unsigned char>(services->fallback_literal[0])
        : services != nullptr
            ? static_cast<unsigned char>(services->fallback_literal_byte)
            : 0U;
    const unsigned char observed = static_cast<unsigned char>(*input);
    return observed == fallback ? 0 : (observed < fallback ? -1 : 1);
}

std::uint32_t FUN_00402350(const FUN_00402350_Object* object) noexcept {
    if (object == nullptr) {
        return 0U;
    }
    return *object->value_at_04;
}

FUN_00402360_String* FUN_00402360(FUN_00402360_String& destination, const char* input) noexcept {
    destination.pointer = nullptr;
    if (input == nullptr || *input == '\0') {
        return &destination;
    }

    const std::size_t length = std::strlen(input);
    const auto* services = g_services;
    if (services == nullptr || services->allocate_aligned == nullptr) {
        return &destination;
    }

    void* allocation = services->allocate_aligned(services->context, length + 0x0CU, 0x10U);
    if (allocation == nullptr) {
        return &destination;
    }

    auto* block = static_cast<FUN_00402360_Block*>(allocation);
    block->ref_count = 1U;
    block->length = static_cast<std::uint32_t>(length);
    std::memcpy(block->data, input, length + 1U);
    destination.pointer = allocation;
    return &destination;
}

void FUN_004023E0(FUN_00402360_String& value) noexcept {
    if (value.pointer == nullptr) {
        return;
    }

    auto* block = static_cast<FUN_00402360_Block*>(value.pointer);
    --block->ref_count;
    if (block->ref_count != 0U) {
        return;
    }

    const auto* services = g_services;
    if (services != nullptr && services->free_aligned != nullptr) {
        services->free_aligned(services->context, value.pointer);
    }
}

const char* FUN_00402420(const FUN_00402360_String& value) noexcept {
    if (value.pointer != nullptr) {
        const auto* block = static_cast<const FUN_00402360_Block*>(value.pointer);
        return block->data;
    }

    const auto* services = g_services;
    return services != nullptr ? services->fallback_literal : nullptr;
}

FUN_00402360_String* FUN_00402430(
    FUN_00402360_String& destination,
    const FUN_00402360_String& source) noexcept {
    FUN_004023E0(destination);
    return FUN_00402360(destination, FUN_00402420(source));
}

} // namespace re5::recovered
