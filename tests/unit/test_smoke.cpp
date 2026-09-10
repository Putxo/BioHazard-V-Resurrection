#include <cstdint>

namespace re5 {
std::uint32_t recovered_runtime_abi_version() noexcept;
}

void test_pilot_bb9070_accessors();
void test_region_00401000();
void test_fun_004014c0();
void test_fun_004015d0();
void test_fun_00401640();
void test_fun_00401650();
void test_fun_00401670();
void test_fun_004016c0();
void test_fun_00401730();
void test_fun_004017b0();
void test_fun_00401830();
void test_fun_00401860();
void test_fun_00401880();
void test_fun_004018c0();
void test_fun_004018f0();
void test_fun_00401910();
void test_fun_00401940();
void test_fun_00401950();
void test_fun_00401b50();
void test_fun_00401d10();
void test_fun_00401d50();
void test_fun_00401e30();
void test_fun_00401e60();
void test_fun_00401fe0();
void test_fun_00402000();

int main() {
    if (re5::recovered_runtime_abi_version() != 1U) return 1;
    test_pilot_bb9070_accessors();
    test_region_00401000();
    test_fun_004014c0();
    test_fun_004015d0();
    test_fun_00401640();
    test_fun_00401650();
    test_fun_00401670();
    test_fun_004016c0();
    test_fun_00401730();
    test_fun_004017b0();
    test_fun_00401830();
    test_fun_00401860();
    test_fun_00401880();
    test_fun_004018c0();
    test_fun_004018f0();
    test_fun_00401910();
    test_fun_00401940();
    test_fun_00401950();
    test_fun_00401b50();
    test_fun_00401d10();
    test_fun_00401d50();
    test_fun_00401e30();
    test_fun_00401e60();
    test_fun_00401fe0();
    test_fun_00402000();
    return 0;
}
