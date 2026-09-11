#pragma once

#include <cstddef>
#include <cstdint>

namespace re5::recovered {

struct FUN_00402480_Services {
    void* context;
    void* (*allocate_aligned)(void* context, std::size_t size, std::size_t alignment) noexcept;
    void* global_16e201c;
    std::uintptr_t (*invoke_00402500)(void* context, void* object, void* argument) noexcept;
};

void FUN_00402480_SetServices(const FUN_00402480_Services* services) noexcept;
void* FUN_00402480(std::size_t size) noexcept;
void* FUN_004024A0() noexcept;
void FUN_004024B0(
    std::uint8_t* base,
    std::uint32_t element_size,
    std::int32_t count,
    void (*destroy)(void* element) noexcept) noexcept;
void FUN_004024F0() noexcept;
std::uintptr_t FUN_00402500(void* object, void* argument) noexcept;
void FUN_00402510() noexcept;

} // namespace re5::recovered
