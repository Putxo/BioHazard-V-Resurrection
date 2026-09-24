#pragma once

#include <cstddef>
#include <cstdint>

namespace re5::recovered {

inline constexpr std::uint32_t FUN_00407E10_ENTRY_BASE = 0x70U;
inline constexpr std::uint32_t FUN_00407E10_ENTRY_STRIDE = 0x8C74U;

// Host backing for two native strides so tests can prove index selection.
// This is not a claim about the native object's total capacity.
inline constexpr std::size_t FUN_00407E10_MODELED_ENTRY_COUNT = 2U;

struct FUN_00407E10_Object {
    std::uint8_t reserved_00_03[0x04];
    std::uint8_t critical_section[0x18];
    std::uint8_t active_flag;
    std::uint8_t reserved_1d_1f[0x03];
    std::uint32_t current_index;
    std::uint8_t reserved_24_6f[0x4C];
    std::uint8_t entry_storage[
        FUN_00407E10_ENTRY_STRIDE * FUN_00407E10_MODELED_ENTRY_COUNT];
};

static_assert(offsetof(FUN_00407E10_Object, critical_section) == 0x04);
static_assert(offsetof(FUN_00407E10_Object, active_flag) == 0x1C);
static_assert(offsetof(FUN_00407E10_Object, current_index) == 0x20);
static_assert(
    offsetof(FUN_00407E10_Object, entry_storage) ==
    FUN_00407E10_ENTRY_BASE);

struct FUN_00407E10_Services {
    void* context;
    std::uint8_t (*read_global_force_flag)(void* context) noexcept;
    void (*enter_critical_section)(
        void* context,
        void* critical_section) noexcept;
    void (*leave_critical_section)(
        void* context,
        void* critical_section) noexcept;
};

void FUN_00407E10_SetServices(
    const FUN_00407E10_Services* services) noexcept;

void FUN_00407E10(
    FUN_00407E10_Object& object,
    std::uint32_t value) noexcept;

} // namespace re5::recovered
