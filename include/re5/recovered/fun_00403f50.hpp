#pragma once

#include <cstddef>
#include <cstdint>

namespace re5::recovered {

struct FUN_00403F50_Object {
    void* state_19c = nullptr;
};

struct FUN_00403F50_Request {
    const char* root;
    const char* suffix;
    const char* category;
};

struct FUN_00403F50_Services {
    const char* active_name;
    const char* fallback_name;
    void* context;
    void* (*allocate_state)(void* context, std::uint32_t bytes) noexcept;
    void (*release_state)(void* context, void* state) noexcept;
    void (*scan_resource)(void* context, FUN_00403F50_Object& object, const FUN_00403F50_Request& request) noexcept;
};

void FUN_00403F50_SetServices(const FUN_00403F50_Services* services) noexcept;
void FUN_00403F50(FUN_00403F50_Object& object) noexcept;

} // namespace re5::recovered
