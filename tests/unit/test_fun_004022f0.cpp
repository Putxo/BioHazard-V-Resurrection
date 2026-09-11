#include "re5/recovered/fun_004022f0.hpp"

#include <cassert>
#include <cstddef>
#include <cstring>

namespace {
struct Probe {
    alignas(16) unsigned char storage[128]{};
    std::size_t requested_size = 0;
    std::size_t requested_alignment = 0;
    int frees = 0;
};

re5::recovered::FUN_004022F0_Buffer* allocate_aligned(void* context, std::size_t size, std::size_t alignment) noexcept {
    auto& probe = *static_cast<Probe*>(context);
    probe.requested_size = size;
    probe.requested_alignment = alignment;
    return reinterpret_cast<re5::recovered::FUN_004022F0_Buffer*>(probe.storage);
}

void free_buffer(void* context, re5::recovered::FUN_004022F0_Buffer*) noexcept {
    ++static_cast<Probe*>(context)->frees;
}
} // namespace

void test_fun_004022f0() {
    using namespace re5::recovered;

    Probe probe{};
    const FUN_004022F0_Services services{&probe, &allocate_aligned, &free_buffer, '\0'};
    FUN_004022F0_SetServices(&services);

    FUN_004022F0_String empty{nullptr};
    assert(FUN_004022F0(empty, "") == 0);
    assert(FUN_004022F0(empty, "x") > 0);
    assert(FUN_00402350(empty) == 0U);

    FUN_004022F0_String owned{nullptr};
    assert(FUN_00402360(owned, "bio5") == &owned);
    assert(owned.buffer != nullptr);
    assert(probe.requested_size == 0x10U);
    assert(probe.requested_alignment == 0x10U);
    assert(owned.buffer->ref_count == 1U);
    assert(owned.buffer->length == 4U);
    assert(std::strcmp(owned.buffer->data, "bio5") == 0);
    assert(FUN_00402350(owned) == 4U);
    assert(FUN_004022F0(owned, "bio5") == 0);
    assert(FUN_004022F0(owned, "bio4") < 0);
    assert(FUN_004022F0(owned, "bio6") > 0);

    FUN_004022F0_String alias{nullptr};
    assert(FUN_004023E0(alias, owned) == &alias);
    assert(alias.buffer == owned.buffer);
    assert(alias.buffer->ref_count == 2U);

    assert(FUN_004023E0(alias, empty) == &alias);
    assert(alias.buffer == nullptr);
    assert(owned.buffer->ref_count == 1U);
    assert(probe.frees == 0);

    assert(FUN_004023E0(owned, empty) == &owned);
    assert(owned.buffer == nullptr);
    assert(probe.frees == 1);

    assert(FUN_00402360(owned, "") == &owned);
    assert(owned.buffer == nullptr);

    FUN_004022F0_SetServices(nullptr);
}
