#include "re5/recovered/fun_00406780.hpp"
#include <cassert>
#include <cstdint>
#include <cstring>
namespace {
struct State { unsigned dispatch_calls=0U; std::int32_t message_index=0; const char* archive_name=nullptr; std::uintptr_t direct_table=0U; std::int32_t direct_count=0; std::uintptr_t resolved_table=1U; std::int32_t resolved_count=1; };
void dispatch_resource_group(void* context,std::int32_t message_index,const char* archive_name,std::uintptr_t direct_table,std::int32_t direct_count,std::uintptr_t resolved_table,std::int32_t resolved_count) noexcept {
    auto& s=*static_cast<State*>(context); ++s.dispatch_calls; s.message_index=message_index; s.archive_name=archive_name; s.direct_table=direct_table; s.direct_count=direct_count; s.resolved_table=resolved_table; s.resolved_count=resolved_count;
}
}
void test_fun_00406780() {
    using namespace re5::recovered;
    FUN_00406780_SetServices(nullptr); FUN_00406780();
    State no_dispatch{}; FUN_00406780_Services none{&no_dispatch,nullptr}; FUN_00406780_SetServices(&none); FUN_00406780(); assert(no_dispatch.dispatch_calls==0U);
    State s{}; FUN_00406780_Services services{&s,dispatch_resource_group}; FUN_00406780_SetServices(&services); FUN_00406780();
    assert(s.dispatch_calls==1U); assert(s.message_index==-1); assert(s.archive_name!=nullptr); assert(std::strcmp(s.archive_name,"Image\\Archive\\Merce2Resource")==0); assert(s.direct_table==0x01567B30U); assert(s.direct_count==13); assert(s.resolved_table==0U); assert(s.resolved_count==0);
    FUN_00406780_SetServices(nullptr);
}
