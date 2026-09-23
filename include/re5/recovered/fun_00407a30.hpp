#pragma once

#include <cstddef>
#include <cstdint>

namespace re5::recovered {

struct FUN_00407A30_Object {
    std::uint8_t reserved_00_27[0x28];
    std::uint32_t state;
};

static_assert(offsetof(FUN_00407A30_Object, state) == 0x28);

struct FUN_00407A30_Services {
    void* context;
    void (*emit_conversion_marker)(void* context, const char* marker) noexcept;
    void (*convert_game_resources)(void* context) noexcept;
    void (*dispatch_manager_slot_24)(void* context, std::uintptr_t token) noexcept;
};

void FUN_00407A30_SetServices(const FUN_00407A30_Services* services) noexcept;
void FUN_00407A30(FUN_00407A30_Object& object) noexcept;

} // namespace re5::recovered
