#include "re5/recovered/fun_00406b20.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace {
constexpr std::array<const char*, 13> k_expected_paths{{
    "id\\jpn\\cockpit\\cp_load_360",
    "id\\eng\\cockpit\\cp_load_360",
    "id\\ger\\cockpit\\cp_load_360",
    "id\\fra\\cockpit\\cp_load_360",
    "id\\spa\\cockpit\\cp_load_360",
    "id\\ita\\cockpit\\cp_load_360",
    "id\\rus\\cockpit\\cp_load_360",
    "id\\pol\\cockpit\\cp_load_360",
    "id\\cze\\cockpit\\cp_load_360",
    "id\\chT\\cockpit\\cp_load_360",
    "id\\chS\\cockpit\\cp_load_360",
    "id\\jpn\\show_E3\\e3_load",
    "id\\eng\\show_E3\\e3_load",
}};

struct State {
    unsigned traces = 0U;
    unsigned formatted = 0U;
    unsigned prepared = 0U;
    unsigned registered = 0U;
    unsigned finalized = 0U;
    unsigned cancel_checks = 0U;
    unsigned loading_gets = 0U;
    unsigned loading_sets = 0U;
    unsigned begin_batches = 0U;
    unsigned start_batches = 0U;
    unsigned end_batches = 0U;
    unsigned binds = 0U;
    unsigned static_index = 0U;
    unsigned message_groups = 0U;
    bool cancel = false;
    std::uint8_t loading = 0U;
    bool saw_end_trace = false;
};

std::uint8_t get_loading(void* c) noexcept {
    auto& s=*static_cast<State*>(c); ++s.loading_gets; return s.loading;
}
void set_loading(void* c, std::uint8_t v) noexcept {
    auto& s=*static_cast<State*>(c); s.loading=v; ++s.loading_sets;
}
void trace_marker(void* c,const char* m) noexcept {
    auto& s=*static_cast<State*>(c); assert(m); ++s.traces;
    if(std::strcmp(m,"0x137AF90")==0) s.saw_end_trace=true;
}
void format_path(void* c,const char* marker,const char* first,const char* second,char* out,std::size_t n) noexcept {
    auto& s=*static_cast<State*>(c); ++s.formatted; assert(marker&&first&&out); assert(n==0x104U);
    const char* value=nullptr;
    if(std::strcmp(marker,"0x137B518")==0){ assert(second==nullptr); assert(std::strcmp(first,"Image\\Archive\\LoadResource")==0); value="primary"; }
    else { assert(std::strcmp(marker,"0x137AAD4")==0); assert(second); assert(std::strcmp(first,"root")==0); assert(std::strcmp(second,"primary")==0); value="secondary"; }
    std::strncpy(out,value,n-1U); out[n-1U]='\0';
}
void prepare_secondary(void* c,const char* p) noexcept { auto& s=*static_cast<State*>(c); assert(std::strcmp(p,"secondary")==0); ++s.prepared; }
void* register_named(void* c,const char* path,std::uintptr_t key,bool enabled) noexcept {
    auto& s=*static_cast<State*>(c); assert(enabled); ++s.registered;
    if(key==0x016E216CU){ assert(std::strcmp(path,"primary")==0); return reinterpret_cast<void*>(0x1000U); }
    assert(key==0x01692E3CU); assert(s.static_index<k_expected_paths.size());
    assert(std::strcmp(path,k_expected_paths[s.static_index])==0); ++s.static_index;
    return reinterpret_cast<void*>(0x2000U+s.static_index);
}
void finalize_registered(void* c,void* p) noexcept { auto& s=*static_cast<State*>(c); assert(p); ++s.finalized; }
bool check_cancel(void* c,const char* p) noexcept { auto& s=*static_cast<State*>(c); assert(std::strcmp(p,"secondary")==0); ++s.cancel_checks; return s.cancel; }
void begin_batch(void* c,const char* p) noexcept { auto& s=*static_cast<State*>(c); assert(std::strcmp(p,"primary")==0); ++s.begin_batches; }
void start_batch(void* c) noexcept { ++static_cast<State*>(c)->start_batches; }
void end_batch(void* c,bool v) noexcept { auto& s=*static_cast<State*>(c); assert(!v); ++s.end_batches; }
void bind_registered(void* c,void** slot) noexcept { auto& s=*static_cast<State*>(c); assert(slot&&*slot); ++s.binds; }
void load_message_group(void* c,std::uint32_t i) noexcept { auto& s=*static_cast<State*>(c); assert(i==s.message_groups); ++s.message_groups; }

re5::recovered::FUN_00406B20_Services make_services(State& s) {
    return {&s,"root",false,false,false,get_loading,set_loading,trace_marker,format_path,prepare_secondary,register_named,finalize_registered,check_cancel,begin_batch,start_batch,end_batch,bind_registered,load_message_group};
}
} // namespace

void test_fun_00406b20() {
    using namespace re5::recovered;

    FUN_00406B20_SetServices(nullptr);
    FUN_00406B20();

    State cancel_state{}; cancel_state.cancel=true;
    auto cancel_services=make_services(cancel_state); cancel_services.global_special_enabled=true;
    FUN_00406B20_SetServices(&cancel_services); FUN_00406B20();
    assert(cancel_state.traces==1U);
    assert(cancel_state.formatted==2U);
    assert(cancel_state.prepared==1U);
    assert(cancel_state.registered==1U);
    assert(cancel_state.finalized==1U);
    assert(cancel_state.cancel_checks==1U);
    assert(cancel_state.loading_gets==0U);
    assert(cancel_state.loading_sets==0U);
    assert(cancel_state.begin_batches==0U);
    assert(cancel_state.start_batches==0U);
    assert(cancel_state.end_batches==0U);
    assert(cancel_state.binds==0U);
    assert(cancel_state.message_groups==0U);
    assert(!cancel_state.saw_end_trace);

    State state{}; state.loading=0x7FU;
    auto services=make_services(state);
    FUN_00406B20_SetServices(&services); FUN_00406B20();
    assert(state.traces==2U && state.saw_end_trace);
    assert(state.formatted==2U);
    assert(state.registered==13U);
    assert(state.static_index==13U);
    assert(state.loading_gets==1U && state.loading_sets==2U && state.loading==0x7FU);
    assert(state.begin_batches==1U && state.start_batches==1U && state.end_batches==1U);
    assert(state.binds==13U);
    assert(state.message_groups==11U);

    State suppressed{};
    auto suppressed_services=make_services(suppressed); suppressed_services.registry_suppressed_a=true;
    FUN_00406B20_SetServices(&suppressed_services); FUN_00406B20();
    assert(suppressed.registered==13U);
    assert(suppressed.binds==0U);
    assert(suppressed.message_groups==11U);

    FUN_00406B20_SetServices(nullptr);
}
