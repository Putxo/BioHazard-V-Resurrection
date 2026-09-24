#include "re5/recovered/fun_00408630.hpp"

#include <cassert>

void test_fun_00408630() {
    using namespace re5::recovered;

    // Host-only safety guard. Native code assumes 0x016E2EFC is mapped.
    FUN_00408630_SetServices(nullptr);
    assert(FUN_00408630() == nullptr);

    FUN_00408630_Services null_value{nullptr};
    FUN_00408630_SetServices(&null_value);
    assert(FUN_00408630() == nullptr);

    int first = 1;
    int second = 2;
    FUN_00408630_Services services{&first};
    FUN_00408630_SetServices(&services);

    assert(FUN_00408630() == &first);
    assert(FUN_00408630() == &first);

    services.global_value = &second;
    assert(FUN_00408630() == &second);

    services.global_value = nullptr;
    assert(FUN_00408630() == nullptr);

    FUN_00408630_SetServices(nullptr);
}
