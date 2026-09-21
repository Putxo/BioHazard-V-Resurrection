#include "re5/recovered/fun_00404e60.hpp"

#include <cassert>
#include <cstring>

namespace {
struct State {
    unsigned scans = 0;
    char first_key[32]{};
    char last_key[32]{};
};

void scan_resource(
    void* context,
    re5::recovered::FUN_00404E60_Object& object,
    const re5::recovered::FUN_00404E60_Request& request) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(object.opaque == &state);
    assert(std::strcmp(request.root, "active") == 0);
    assert(std::strcmp(request.root_suffix, "\\effect") == 0);
    if (state.scans == 0U) {
        std::strncpy(state.first_key, request.key, sizeof(state.first_key) - 1U);
    }
    std::strncpy(state.last_key, request.key, sizeof(state.last_key) - 1U);
    ++state.scans;
}
} // namespace

void test_fun_00404e60() {
    using namespace re5::recovered;

    State state{};
    FUN_00404E60_Services services{
        "active",
        "",
        &state,
        scan_resource,
    };
    FUN_00404E60_SetServices(&services);

    FUN_00404E60_Object object{&state};
    const bool ok = FUN_00404E60(object);

    assert(ok);
    assert(state.scans == 6U);
    assert(std::strcmp(state.first_key, "0x137AF48") == 0);
    assert(std::strcmp(state.last_key, "0x137AF04") == 0);

    FUN_00404E60_SetServices(nullptr);
}
