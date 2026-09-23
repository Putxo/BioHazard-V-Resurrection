#pragma once
#include <cstdint>
namespace re5::recovered {
struct FUN_00406DB0_Services {
    void* context;
    void (*dispatch_resource_group)(void*,std::int32_t,const char*,std::uintptr_t,std::int32_t,std::uintptr_t,std::int32_t) noexcept;
    void (*trace_marker)(void*,const char*) noexcept;
    void (*load_message_group)(void*,std::uint32_t) noexcept;
    void (*load_game2_resource)(void*) noexcept;
    void (*scan_resource_tree)(void*,const char*,const char*,const char*) noexcept;
};
void FUN_00406DB0_SetServices(const FUN_00406DB0_Services*) noexcept;
void FUN_00406DB0() noexcept;
}
