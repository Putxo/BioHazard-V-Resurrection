#include "re5/recovered/fun_00402480.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>

namespace {
struct State {
    std::size_t size = 0;
    std::size_t alignment = 0;
    unsigned allocate_calls = 0;
    unsigned thunk_calls = 0;
    void* thunk_object = nullptr;
    void* thunk_argument = nullptr;
    std::array<std::byte, 64> storage{};
};

State* g_destroy_state = nullptr;
std::uint8_t* g_destroy_base = nullptr;
std::array<std::ptrdiff_t, 8> g_destroy_offsets{};
unsigned g_destroy_calls = 0;

void* allocate_aligned(void* context, std::size_t size, std::size_t alignment) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.allocate_calls;
    state.size = size;
    state.alignment = alignment;
    return state.storage.data();
}

std::uintptr_t invoke_thunk(void* context, void* object, void* argument) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.thunk_calls;
    state.thunk_object = object;
    state.thunk_argument = argument;
    return 0xA5A55A5AU;
}

void destroy_element(void* element) noexcept {
    (void)g_destroy_state;
    g_destroy_offsets[g_destroy_calls++] = static_cast<std::uint8_t*>(element) - g_destroy_base;
}
} // namespace

void test_fun_00402480() {
    using namespace re5::recovered;

    State state{};
    int global_value = 7;
    FUN_00402480_Services services{&state, allocate_aligned, &global_value, invoke_thunk};
    FUN_00402480_SetServices(&services);

    assert(FUN_00402480(0x123U) == state.storage.data());
    assert(state.allocate_calls == 1U);
    assert(state.size == 0x123U);
    assert(state.alignment == 0x10U);
    assert(FUN_004024A0() == &global_value);

    std::array<std::uint8_t, 16> elements{};
    g_destroy_state = &state;
    g_destroy_base = elements.data();
    g_destroy_calls = 0;
    FUN_004024B0(elements.data(), 4U, 3, destroy_element);
    assert(g_destroy_calls == 3U);
    assert(g_destroy_offsets[0] == 8);
    assert(g_destroy_offsets[1] == 4);
    assert(g_destroy_offsets[2] == 0);

    g_destroy_calls = 0;
    FUN_004024B0(elements.data(), 4U, 0, destroy_element);
    assert(g_destroy_calls == 0U);

    FUN_004024F0();
    int object = 1;
    int argument = 2;
    assert(FUN_00402500(&object, &argument) == 0xA5A55A5AU);
    assert(state.thunk_calls == 1U);
    assert(state.thunk_object == &object);
    assert(state.thunk_argument == &argument);
    FUN_00402510();

    FUN_00402480_SetServices(nullptr);
    assert(FUN_00402480(16U) == nullptr);
    assert(FUN_004024A0() == nullptr);
    assert(FUN_00402500(nullptr, nullptr) == 0U);
}
