#pragma once

#include <cstdint>

namespace re5::recovered {

struct FUN_00403AF0_NameRef {
    const char* text;
};

struct FUN_00403AF0_Entry {
    const char* name;
    bool descend;
};

struct FUN_00403AF0_TableRecord {
    const char* name;
    const void* object;
    const char* suffix;
};

struct FUN_00403AF0_Services {
    const char* fallback_name;
    std::uintptr_t registry_root_016e20dc;
    void* context;
    bool (*validate_name)(void* context, const char* name) noexcept;
    bool (*registry_ready)(void* context) noexcept;
    void (*ensure_registry)(void* context, std::uintptr_t registry) noexcept;
    bool (*begin_scan)(void* context, const char* root_name) noexcept;
    bool (*next_entry)(void* context, FUN_00403AF0_Entry& entry) noexcept;
    bool (*end_scan)(void* context) noexcept;
    const FUN_00403AF0_TableRecord* (*find_record)(void* context, const char* leaf) noexcept;
    void (*bind_record)(void* context, const FUN_00403AF0_TableRecord& record, const char* path) noexcept;
    void (*report_missing)(void* context, const char* path) noexcept;
};

void FUN_00403AF0_SetServices(const FUN_00403AF0_Services* services) noexcept;
void FUN_00403AF0(FUN_00403AF0_NameRef* name) noexcept;

} // namespace re5::recovered
