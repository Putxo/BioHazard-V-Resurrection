#include "re5/recovered/fun_00402250.hpp"

#include <cassert>

namespace {
re5::recovered::FUN_00402250_Object* virtual_slot_10(void* self) noexcept {
    return static_cast<re5::recovered::FUN_00402250_Object*>(self);
}
} // namespace

void test_fun_00402250() {
    using namespace re5::recovered;

    FUN_00402250_SetServices(nullptr);
    assert(FUN_00402250(nullptr) == 0U);
    assert(FUN_00402260() == 0x01655B5CU);
    assert(FUN_00402270() == nullptr);
    assert(FUN_00402280() == 0x016E1D3CU);

    FUN_00402250_Object object{nullptr, 0x10203040U};
    int tail_2270 = 1;
    int tail_2290 = 2;
    const FUN_00402250_Services services{&tail_2270, &tail_2290, &virtual_slot_10};
    FUN_00402250_SetServices(&services);

    assert(FUN_00402250(&object) == 0x10203040U);
    assert(FUN_00402270() == &tail_2270);
    assert(FUN_00402290(&object) == &tail_2290);
    assert(object.vtable == reinterpret_cast<void*>(0x01541F48U));

    FUN_004022A0_Bytes bytes{};
    assert(FUN_004022A0(bytes, 0x22U, 0x11U, 0x00U, 0x33U) == &bytes);
    assert(bytes.b0 == 0x00U);
    assert(bytes.b1 == 0x22U);
    assert(bytes.b2 == 0x11U);
    assert(bytes.b3 == 0x33U);

    assert(FUN_004022C0(bytes) == &bytes);
    assert(bytes.b0 == 0U);
    assert(bytes.b1 == 0U);
    assert(bytes.b2 == 0U);
    assert(bytes.b3 == 0U);

    std::uint32_t count = 7U;
    const FUN_004022D0_Source source{&count};
    FUN_004022D0_Source destination{nullptr};
    assert(FUN_004022D0(destination, source) == &destination);
    assert(destination.pointer == &count);
    assert(count == 8U);

    const FUN_004022D0_Source null_source{nullptr};
    assert(FUN_004022D0(destination, null_source) == &destination);
    assert(destination.pointer == nullptr);

    FUN_00402250_SetServices(nullptr);
}
