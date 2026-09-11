#include "re5/recovered/fun_00402120.hpp"

#include <cassert>
#include <cstddef>

namespace {
struct Probe {
    std::size_t size = 0;
    std::size_t alignment = 0;
    int result = 0;
};

void* allocate_aligned(void* context, std::size_t size, std::size_t alignment) noexcept {
    auto& probe = *static_cast<Probe*>(context);
    probe.size = size;
    probe.alignment = alignment;
    return &probe.result;
}
} // namespace

void test_fun_00402120() {
    using namespace re5::recovered;

    FUN_00402120_SetServices(nullptr);
    assert(FUN_00402120(0x28U) == nullptr);

    Probe probe{};
    const FUN_00402120_Services services{&probe, &allocate_aligned};
    FUN_00402120_SetServices(&services);

    assert(FUN_00402120(0x28U) == &probe.result);
    assert(probe.size == 0x28U);
    assert(probe.alignment == 0x10U);

    FUN_00402120_SetServices(nullptr);
}
