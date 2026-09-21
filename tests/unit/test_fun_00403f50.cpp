#include "re5/recovered/fun_00403f50.hpp"

#include <cassert>
#include <cstring>

namespace {
struct State {
    int state_token = 0x3f50;
    unsigned allocs = 0;
    unsigned releases = 0;
    unsigned scans = 0;
    char first_suffix[32]{};
    char last_suffix[32]{};
};

void* allocate_state(void* context, std::uint32_t bytes) noexcept {
    assert(bytes == 0x130U);
    auto& state = *static_cast<State*>(context);
    ++state.allocs;
    return &state.state_token;
}

void release_state(void* context, void* ptr) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(ptr == &state.state_token);
    ++state.releases;
}

void scan_resource(
    void* context,
    re5::recovered::FUN_00403F50_Object& object,
    const re5::recovered::FUN_00403F50_Request& request) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(object.state_19c == &state.state_token);
    assert(std::strcmp(request.root, "active") == 0);
    if (state.scans == 0U) {
        std::strncpy(state.first_suffix, request.suffix, sizeof(state.first_suffix) - 1U);
    }
    std::strncpy(state.last_suffix, request.suffix, sizeof(state.last_suffix) - 1U);
    ++state.scans;
}
} // namespace

void test_fun_00403f50() {
    using namespace re5::recovered;

    State state{};
    FUN_00403F50_Services services{
        "active",
        "",
        &state,
        allocate_state,
        release_state,
        scan_resource,
    };
    FUN_00403F50_SetServices(&services);

    FUN_00403F50_Object object{};
    FUN_00403F50(object);

    assert(state.allocs == 1U);
    assert(state.releases == 1U);
    assert(state.scans == 56U);
    assert(std::strcmp(state.first_suffix, "0x137AF48") == 0);
    assert(std::strcmp(state.last_suffix, "0x137AE5C") == 0);
    assert(object.state_19c == nullptr);

    FUN_00403F50_SetServices(nullptr);
}
