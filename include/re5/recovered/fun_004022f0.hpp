#pragma once

#include <cstddef>
#include <cstdint>

namespace re5::recovered {

struct FUN_004022F0_Object {
    const void* field_00;
    const char* string_at_08;
};

struct FUN_00402350_Object {
    const void* field_00;
    const std::uint32_t* value_at_04;
};

struct FUN_00402360_String {
    void* pointer;
};

struct FUN_00402360_Block {
    std::uint32_t ref_count;
    std::uint32_t length;
    char data[1];
};

struct FUN_00402360_Services {
    char fallback_literal_byte;
    void* context;
    void* (*allocate_aligned)(void* context, std::size_t size, std::size_t alignment) noexcept;
    void (*free_aligned)(void* context, void* pointer) noexcept;
};

void FUN_00402360_SetServices(const FUN_00402360_Services* services) noexcept;
int FUN_004022F0(const FUN_004022F0_Object* object, const char* input) noexcept;
std::uint32_t FUN_00402350(const FUN_00402350_Object* object) noexcept;
FUN_00402360_String* FUN_00402360(FUN_00402360_String& destination, const char* input) noexcept;
void FUN_004023E0(FUN_00402360_String& value) noexcept;

} // namespace re5::recovered
