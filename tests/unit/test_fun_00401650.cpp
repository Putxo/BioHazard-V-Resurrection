#include "re5/recovered/fun_00401650.hpp"

#include <cassert>
#include <cstdint>

namespace {
struct Context {
    void* object = nullptr;
    std::uint32_t result = 0;
    std::uint32_t argument = 0;
    int provider_calls = 0;
    int slot_calls = 0;
};

void* get_user_stats(void* opaque) noexcept {
    auto* context = static_cast<Context*>(opaque);
    ++context->provider_calls;
    return context->object;
}

std::uint32_t invoke_slot_54(void* object, std::uint32_t argument) noexcept {
    auto* context = static_cast<Context*>(object);
    ++context->slot_calls;
    context->argument = argument;
    return context->result;
}
} // namespace

void test_fun_00401650() {
    using namespace re5::recovered;

    Context context{};
    context.object = &context;
    context.result = 0xA5A55A5AU;
    const FUN_00401650_Services services{&context, get_user_stats, invoke_slot_54};
    FUN_00401650_SetServices(&services);

    assert(FUN_00401650(0x12345678U) == 0xA5A55A5AU);
    assert(context.provider_calls == 1);
    assert(context.slot_calls == 1);
    assert(context.argument == 0x12345678U);

    FUN_00401650_SetServices(nullptr);
    assert(FUN_00401650(1U) == 0U);
}
