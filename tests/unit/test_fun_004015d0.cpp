#include "re5/recovered/fun_004015d0.hpp"

#include <cassert>
#include <cstddef>

namespace {
struct Context {
    const char* country = nullptr;
    std::size_t calls = 0;
};

const char* get_ip_country(void* opaque) noexcept {
    auto* context = static_cast<Context*>(opaque);
    ++context->calls;
    return context->country;
}
} // namespace

void test_fun_004015d0() {
    using namespace re5::recovered;

    Context context{};
    const FUN_004015D0_Services services{&context, get_ip_country};
    FUN_004015D0_SetServices(&services);

    context.country = "AU";
    assert(FUN_004015D0() == 1);
    assert(context.calls == 1);

    context.calls = 0;
    context.country = "GB";
    assert(FUN_004015D0() == 13);
    assert(context.calls == 1);

    context.calls = 0;
    context.country = "UK";
    assert(FUN_004015D0() == 13);
    assert(context.calls == 1);

    context.calls = 0;
    context.country = "ES";
    assert(FUN_004015D0() == 41);
    assert(context.calls == 1);

    context.calls = 0;
    context.country = "US";
    assert(FUN_004015D0() == 47);
    assert(context.calls == 1);

    context.calls = 0;
    context.country = "XX";
    assert(FUN_004015D0() == 48);
    assert(context.calls == 1);

    context.calls = 0;
    context.country = "ZZ";
    assert(FUN_004015D0() == 48);
    assert(context.calls == 1);

    context.calls = 0;
    context.country = nullptr;
    assert(FUN_004015D0() == 48);
    assert(context.calls == 1);

    FUN_004015D0_SetServices(nullptr);
    assert(FUN_004015D0() == 48);
}
