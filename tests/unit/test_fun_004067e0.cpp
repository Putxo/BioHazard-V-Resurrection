#include "re5/recovered/fun_004067e0.hpp"
#include <cassert>
#include <cstdint>
#include <cstring>
namespace {
struct State { unsigned calls=0; std::int32_t index=0; const char* name=nullptr; std::uintptr_t direct=0; std::int32_t direct_count=0; std::uintptr_t resolved=1; std::int32_t resolved_count=1; };
void dispatch(void* c,std::int32_t i,const char* n,std::uintptr_t d,std::int32_t dc,std::uintptr_t r,std::int32_t rc) noexcept { auto& s=*static_cast<State*>(c); ++s.calls; s.index=i; s.name=n; s.direct=d; s.direct_count=dc; s.resolved=r; s.resolved_count=rc; }
}
void test_fun_004067e0() {
    using namespace re5::recovered;
    FUN_004067E0_SetServices(nullptr); FUN_004067E0();
    State no_dispatch{}; FUN_004067E0_Services nd{&no_dispatch,nullptr}; FUN_004067E0_SetServices(&nd); FUN_004067E0(); assert(no_dispatch.calls==0);
    State s{}; FUN_004067E0_Services svc{&s,dispatch}; FUN_004067E0_SetServices(&svc); FUN_004067E0();
    assert(s.calls==1); assert(s.index==-1); assert(s.name && std::strcmp(s.name,"Image\\Archive\\FigureResource")==0); assert(s.direct==0x01567BC8U); assert(s.direct_count==7); assert(s.resolved==0U); assert(s.resolved_count==0);
    FUN_004067E0_SetServices(nullptr);
}
