#pragma once

#include <cstddef>
#include <cstdint>

namespace re5::recovered {

using FUN_004011B0_Callback = void (*)(void*) noexcept;

void FUN_00401000(void* ptr) noexcept;
void FUN_00401010(char16_t* title_buffer, const char* product_name, bool apply_to_window) noexcept;

void FUN_004011B0(
    void* first,
    std::uint32_t stride,
    std::uint32_t count,
    FUN_004011B0_Callback callback) noexcept;

[[nodiscard]] std::uintptr_t FUN_004011F0() noexcept;
[[nodiscard]] void* FUN_00401200(std::size_t size) noexcept;
void FUN_00401210(std::uintptr_t unused, const char* text) noexcept;
[[nodiscard]] bool FUN_00401220() noexcept;
void FUN_00401230(std::uintptr_t unused) noexcept;
void FUN_00401240(std::uintptr_t unused) noexcept;

// 0x00401250 / 0x00401260: exact steam_api!SteamAPI_RunCallbacks tail-call thunks.
void FUN_00401250() noexcept;
void FUN_00401260() noexcept;

struct FUN_00401270_UserSigninInfo {
    std::uint64_t xuid;
    std::uint32_t info_flags;
    std::uint32_t signin_state;
    std::uint32_t guest_number;
    std::uint32_t sponsor_user_index;
    char user_name[16];
};

struct FUN_00401270_Achievement {
    std::uint32_t user_index;
    std::uint32_t achievement_id;
};

static_assert(sizeof(FUN_00401270_UserSigninInfo) == 0x28U);
static_assert(offsetof(FUN_00401270_UserSigninInfo, signin_state) == 0x0CU);
static_assert(offsetof(FUN_00401270_UserSigninInfo, user_name) == 0x18U);
static_assert(sizeof(FUN_00401270_Achievement) == 8U);

struct FUN_00401270_Services {
    void* context;
    void (*prepare_user_state)(void* context) noexcept;
    std::uint32_t (*get_signin_info)(
        void* context,
        std::uint32_t user_index,
        std::uint32_t flags,
        FUN_00401270_UserSigninInfo* info) noexcept;
    void (*log_not_live)(void* context, const char* user_name) noexcept;
    std::uint32_t (*write_achievements)(
        void* context,
        std::uint32_t count,
        const FUN_00401270_Achievement* achievements,
        void* overlapped) noexcept;
    void (*mirror_achievement)(void* context, std::uint32_t achievement_id) noexcept;
};

// Host-side dependency binding for the two original globals and xlive imports.
// The original entry has no arguments; LINKED remains false until target bindings exist.
void FUN_00401270_SetServices(const FUN_00401270_Services* services) noexcept;
void FUN_00401270() noexcept;

struct FUN_004012F0_Services {
    void* context;
    std::uint32_t (*get_signin_info)(
        void* context,
        std::uint32_t user_index,
        std::uint32_t flags,
        FUN_00401270_UserSigninInfo* info) noexcept;
    std::uint32_t (*create_achievement_enumerator)(
        void* context,
        std::uint32_t title_id,
        std::uint32_t user_index,
        std::uint64_t xuid,
        std::uint32_t detail_flags,
        std::uint32_t starting_index,
        std::uint32_t item_count,
        std::uint32_t* buffer_size,
        std::uint32_t* handle) noexcept;
    std::uint32_t (*enumerate)(
        void* context,
        std::uint32_t handle,
        void* buffer,
        std::uint32_t buffer_size,
        std::uint32_t* items_returned,
        void* overlapped) noexcept;
    void (*close_handle)(void* context, std::uint32_t handle) noexcept;
};

void FUN_004012F0_SetServices(const FUN_004012F0_Services* services) noexcept;
[[nodiscard]] std::uint32_t FUN_004012F0(
    void** out_buffer,
    std::uint32_t requested_count,
    std::uint32_t unused) noexcept;

struct FUN_00401400_Object {
    std::uint32_t unknown_00;
    std::uint32_t current;
    std::uint32_t target;
};

static_assert(offsetof(FUN_00401400_Object, current) == 0x04U);
static_assert(offsetof(FUN_00401400_Object, target) == 0x08U);

[[nodiscard]] bool FUN_00401400(const FUN_00401400_Object* self) noexcept;

struct FUN_00401410_Services {
    void* context;
    void* (*get_object)(void* context) noexcept;
    bool (*query_threshold)(
        void* object,
        std::uintptr_t key,
        std::uint32_t threshold) noexcept;
    void (*dispatch)(void* object) noexcept;
};

void FUN_00401410_SetServices(const FUN_00401410_Services* services) noexcept;
void FUN_00401410() noexcept;

} // namespace re5::recovered
