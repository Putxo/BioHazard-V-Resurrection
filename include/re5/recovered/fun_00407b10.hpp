#pragma once

#include <cstddef>
#include <cstdint>

namespace re5::recovered {

struct FUN_00407B10_Object {
    std::uint8_t reserved_00_03[0x04];
    std::uint8_t critical_section[0x18];
    std::uint8_t active_flag;
};

static_assert(offsetof(FUN_00407B10_Object, critical_section) == 0x04);
static_assert(offsetof(FUN_00407B10_Object, active_flag) == 0x1C);

struct FUN_00407B10_Services {
    void* context;
    std::uint8_t (*read_global_force_flag)(void* context) noexcept;
    void (*enter_critical_section)(void* context, void* critical_section) noexcept;
};

void FUN_00407B10_SetServices(const FUN_00407B10_Services* services) noexcept;
void FUN_00407B10(FUN_00407B10_Object& object) noexcept;

} // namespace re5::recovered
