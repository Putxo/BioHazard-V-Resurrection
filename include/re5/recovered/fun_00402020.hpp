#pragma once

#include <cstdint>

namespace re5::recovered {

struct FUN_00402020_CallbackSlot {
    const char* name;
    bool active;
    std::uint32_t value;
    void* owner;
    std::uintptr_t callback_va;
};

struct FUN_00402020_Services {
    void* context;
    void (*unregister_callback)(void* context, FUN_00402020_CallbackSlot* slot) noexcept;
    void (*register_callback)(void* context, FUN_00402020_CallbackSlot* slot, std::uint32_t callback_id) noexcept;
};

void FUN_00402020_SetServices(const FUN_00402020_Services* services) noexcept;
void FUN_00402020(FUN_00402020_CallbackSlot& slot, void* owner, std::uintptr_t callback_va) noexcept;

} // namespace re5::recovered
