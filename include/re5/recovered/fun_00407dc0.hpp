#pragma once

#include <cstddef>
#include <cstdint>

namespace re5::recovered {

inline constexpr std::uint32_t FUN_00407DC0_ENTRY_BASE = 0x34U;
inline constexpr std::uint32_t FUN_00407DC0_ENTRY_STRIDE = 0x5E0U;

// Host-side backing used only to make the recovered direct-store contract
// testable. This is not a claim about the native container's total capacity.
inline constexpr std::size_t FUN_00407DC0_MODELED_ENTRY_COUNT = 2U;

struct FUN_00407DC0_Object {
    std::uint8_t reserved_00_03[0x04];
    std::uint8_t critical_section[0x18];
    std::uint8_t active_flag;
    std::uint8_t reserved_1d_33[0x17];
    std::uint8_t entry_storage[
        FUN_00407DC0_ENTRY_STRIDE * FUN_00407DC0_MODELED_ENTRY_COUNT];
};

static_assert(offsetof(FUN_00407DC0_Object, critical_section) == 0x04);
static_assert(offsetof(FUN_00407DC0_Object, active_flag) == 0x1C);
static_assert(
    offsetof(FUN_00407DC0_Object, entry_storage) ==
    FUN_00407DC0_ENTRY_BASE);

struct FUN_00407DC0_Services {
    void* context;
    std::uint8_t (*read_global_force_flag)(void* context) noexcept;
    void (*enter_critical_section)(
        void* context,
        void* critical_section) noexcept;
    void (*leave_critical_section)(
        void* context,
        void* critical_section) noexcept;
};

void FUN_00407DC0_SetServices(
    const FUN_00407DC0_Services* services) noexcept;

void FUN_00407DC0(
    FUN_00407DC0_Object& object,
    std::uint32_t index,
    std::uint32_t value) noexcept;

} // namespace re5::recovered
