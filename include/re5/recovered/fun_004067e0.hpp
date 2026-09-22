#pragma once
#include <cstdint>
namespace re5::recovered {
struct FUN_004067E0_Services { void* context; void (*dispatch_resource_group)(void*, std::int32_t, const char*, std::uintptr_t, std::int32_t, std::uintptr_t, std::int32_t) noexcept; };
void FUN_004067E0_SetServices(const FUN_004067E0_Services*) noexcept;
void FUN_004067E0() noexcept;
}
