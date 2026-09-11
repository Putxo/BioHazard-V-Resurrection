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
        return std::strcmp(stored, input);
    }

    const auto* services = g_services;
    const unsigned char fallback = services != nullptr
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

} // namespace re5::recovered
