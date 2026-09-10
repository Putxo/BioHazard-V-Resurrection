#include <cstdint>

namespace re5 {
std::uint32_t recovered_runtime_abi_version() noexcept;
}

void test_pilot_bb9070_accessors();
void test_region_00401000();
void test_fun_004014c0();

int main() {
    if (re5::recovered_runtime_abi_version() != 1U) return 1;
    test_pilot_bb9070_accessors();
    test_region_00401000();
    test_fun_004014c0();
    return 0;
}
