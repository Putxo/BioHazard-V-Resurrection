#pragma once

#include "re5/recovered/fun_00401b50.hpp"

#include <cstdint>

namespace re5::recovered {

struct FUN_00401D50_CallbackSlot {
    const char* name;
    bool active;
    std::uint32_t value;
    void* owner;
    std::uintptr_t callback_va;
};

struct FUN_00401D50_Object {
    FUN_00401B50_State summary;
    FUN_00401D50_CallbackSlot slot_10;
    FUN_00401D50_CallbackSlot slot_24;
    FUN_00401D50_CallbackSlot slot_38;
};

struct FUN_00401D50_Services {
    void* context;
    void (*publish_global_instance)(void* context, FUN_00401D50_Object* object) noexcept;
    void (*register_callback)(
        void* context,
        FUN_00401D50_Object* object,
        FUN_00401D50_CallbackSlot* slot,
        std::uintptr_t callback_va) noexcept;
    bool (*has_user_stats)(void* context) noexcept;
    bool (*has_user)(void* context) noexcept;
    bool (*is_user_logged_on)(void* context) noexcept;
    void (*request_current_stats)(void* context) noexcept;
};

void FUN_00401D50_SetServices(const FUN_00401D50_Services* services) noexcept;
FUN_00401D50_Object* FUN_00401D50(FUN_00401D50_Object& object) noexcept;

} // namespace re5::recovered
