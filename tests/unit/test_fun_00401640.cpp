#include "re5/recovered/fun_00401640.hpp"

void test_fun_00401640() {
    // Non-Windows CI exercises the recovered thunk through the host adapter.
    // The adapter intentionally has no external side effect when steam_api.dll
    // is unavailable; this verifies that the recovered entry remains callable.
    re5::recovered::FUN_00401640();
}
