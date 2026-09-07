#include <cstdint>

namespace re5 {
std::uint32_t recovered_runtime_abi_version() noexcept;
}

void test_pilot_bb9070_accessors();

int main() {
    if (re5::recovered_runtime_abi_version() != 1U) return 1;
    test_pilot_bb9070_accessors();
    return 0;
}
