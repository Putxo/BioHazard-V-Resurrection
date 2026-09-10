#include "re5/recovered/fun_004017b0.hpp"

#include <array>
#include <cassert>
#include <cstdint>

namespace {
struct Context {
    std::array<std::uint8_t, 8> seed{1,2,3,4,5,6,7,8};
    int seed_reads = 0;
};

void get_seed8(void* opaque, std::uint8_t* out) noexcept {
    auto& ctx = *static_cast<Context*>(opaque);
    ++ctx.seed_reads;
    for (std::size_t i = 0; i < ctx.seed.size(); ++i) out[i] = ctx.seed[i];
}
}

void test_fun_004017b0() {
    using namespace re5::recovered;

    Context ctx{};
    const FUN_004017B0_Services services{&ctx, &get_seed8};
    FUN_004017B0_SetServices(&services);

    const std::array<std::uint8_t, 10> ciphertext{0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x00,0x00};
    const std::array<std::uint8_t, 10> expected{0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80,0x11,0x22};
    std::array<std::uint8_t, 10> output{};

    assert(FUN_004017B0(output.data(), static_cast<std::uint32_t>(output.size()), ciphertext.data(), static_cast<std::uint32_t>(ciphertext.size())) == ciphertext.size());
    assert(output == expected);
    assert(ctx.seed_reads == 1);

    std::array<std::uint8_t, 1> small{};
    assert(FUN_004017B0(small.data(), 1U, ciphertext.data(), 2U) == 0U);
    assert(ctx.seed_reads == 1);

    assert(FUN_004017B0(output.data(), static_cast<std::uint32_t>(output.size()), ciphertext.data(), 0U) == 0U);
    assert(ctx.seed_reads == 2);

    FUN_004017B0_SetServices(nullptr);
    assert(FUN_004017B0(output.data(), static_cast<std::uint32_t>(output.size()), ciphertext.data(), 1U) == 0U);
}
