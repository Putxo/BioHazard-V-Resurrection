#include "re5/recovered/fun_004022f0.hpp"

#include <cstring>

namespace re5::recovered {
namespace {
const FUN_004022F0_Services* g_services = nullptr;

int compare_bytes(const char* lhs, const char* rhs) noexcept {
    const auto* a = reinterpret_cast<const unsigned char*>(lhs != nullptr ? lhs : "");
    const auto* b = reinterpret_cast<const unsigned char*>(rhs != nullptr ? rhs : "");

    while (*a == *b) {
        if (*a == 0U) {
            return 0;
        }
        ++a;
        ++b;
    }

    return *a < *b ? -1 : 1;
}

void release_buffer(FUN_004022F0_Buffer* buffer) noexcept {
    if (buffer == nullptr) {
        return;
    }

    --buffer->ref_count;
    if (buffer->ref_count == 0U) {
        const auto* services = g_services;
        if (services != nullptr && services->free_buffer != nullptr) {
            services->free_buffer(services->allocator_context, buffer);
        }
    }
}
}

void FUN_004022F0_SetServices(const FUN_004022F0_Services* services) noexcept {
    g_services = services;
}

int FUN_004022F0(const FUN_004022F0_String& self, const char* text) noexcept {
    const auto* buffer = self.buffer;
    if (buffer != nullptr) {
        return compare_bytes(text, buffer->data);
    }

    const auto* services = g_services;
    const char fallback[2]{services != nullptr ? services->fallback_byte : '\0', '\0'};
    const unsigned char lhs = static_cast<unsigned char>(text != nullptr ? text[0] : '\0');
    const unsigned char rhs = static_cast<unsigned char>(fallback[0]);

    if (lhs == rhs) {
        return 0;
    }
    return lhs < rhs ? -1 : 1;
}

std::uint32_t FUN_00402350(const FUN_004022F0_String& self) noexcept {
    return self.buffer != nullptr ? self.buffer->length : 0U;
}

FUN_004022F0_String* FUN_00402360(FUN_004022F0_String& self, const char* text) noexcept {
    if (text == nullptr || text[0] == '\0') {
        self.buffer = nullptr;
        return &self;
    }

    const auto length = std::strlen(text);
    const auto* services = g_services;
    if (services == nullptr || services->allocate_aligned == nullptr) {
        self.buffer = nullptr;
        return &self;
    }

    auto* buffer = services->allocate_aligned(services->allocator_context, length + 0x0CU, 0x10U);
    self.buffer = buffer;
    if (buffer == nullptr) {
        return &self;
    }

    buffer->ref_count = 1U;
    buffer->length = static_cast<std::uint32_t>(length);
    std::memcpy(buffer->data, text, length + 1U);
    return &self;
}

FUN_004022F0_String* FUN_004023E0(FUN_004022F0_String& self, const FUN_004022F0_String& source) noexcept {
    release_buffer(self.buffer);
    self.buffer = source.buffer;
    if (self.buffer != nullptr) {
        ++self.buffer->ref_count;
    }
    return &self;
}

} // namespace re5::recovered
