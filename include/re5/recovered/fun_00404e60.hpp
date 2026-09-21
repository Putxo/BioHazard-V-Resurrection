#pragma once

namespace re5::recovered {

struct FUN_00404E60_Object {
    void* opaque = nullptr;
};

struct FUN_00404E60_Request {
    const char* root;
    const char* root_suffix;
    const char* key;
};

struct FUN_00404E60_Services {
    const char* active_name;
    const char* fallback_name;
    void* context;
    void (*scan_resource)(void* context, FUN_00404E60_Object& object, const FUN_00404E60_Request& request) noexcept;
};

void FUN_00404E60_SetServices(const FUN_00404E60_Services* services) noexcept;
bool FUN_00404E60(FUN_00404E60_Object& object) noexcept;

} // namespace re5::recovered
