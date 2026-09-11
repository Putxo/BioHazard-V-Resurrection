#pragma once

#include <cstddef>
#include <cstdint>

namespace re5::recovered {

struct FUN_004022F0_Buffer {
    std::uint32_t ref_count;
    std::uint32_t length;
    char data[1];
};

struct FUN_004022F0_String {
    FUN_004022F0_Buffer* buffer;
};

struct FUN_004022F0_Services {
    void* allocator_context;
    FUN_004022F0_Buffer* (*allocate_aligned)(void* allocator_context, std::size_t size, std::size_t alignment) noexcept;
    void (*free_buffer)(void* allocator_context, FUN_004022F0_Buffer* buffer) noexcept;
    char fallback_byte;
};

void FUN_004022F0_SetServices(const FUN_004022F0_Services* services) noexcept;
int FUN_004022F0(const FUN_004022F0_String& self, const char* text) noexcept;
std::uint32_t FUN_00402350(const FUN_004022F0_String& self) noexcept;
FUN_004022F0_String* FUN_00402360(FUN_004022F0_String& self, const char* text) noexcept;
FUN_004022F0_String* FUN_004023E0(FUN_004022F0_String& self, const FUN_004022F0_String& source) noexcept;

} // namespace re5::recovered
