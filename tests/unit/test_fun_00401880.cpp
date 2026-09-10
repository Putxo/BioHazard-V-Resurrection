#include "re5/recovered/fun_00401880.hpp"

#include <cassert>
#include <cstring>

namespace {
int g_provider_calls = 0;
int g_slot_calls = 0;
void* g_friends = reinterpret_cast<void*>(0x1234);
const char* g_text = nullptr;

void* get_friends(void*) noexcept {
    ++g_provider_calls;
    return g_friends;
}

const char* invoke_slot_00(void*, void* friends) noexcept {
    ++g_slot_calls;
    assert(friends == g_friends);
    return g_text;
}
}

void test_fun_00401880() {
    using namespace re5::recovered;
    const FUN_00401880_Services services{nullptr, &get_friends, &invoke_slot_00};
    FUN_00401880_SetServices(&services);

    char buffer[8];
    std::memset(buffer, 'X', sizeof(buffer));
    g_provider_calls = g_slot_calls = 0;
    g_friends = reinterpret_cast<void*>(0x1234);
    g_text = "Ada";
    assert(FUN_00401880(buffer, sizeof(buffer)) == buffer);
    assert(std::strcmp(buffer, "Ada") == 0);
    assert(buffer[4] == '\0');
    assert(buffer[7] == '\0');
    assert(g_provider_calls == 1 && g_slot_calls == 1);

    std::memset(buffer, 'X', sizeof(buffer));
    g_text = "123456789";
    assert(FUN_00401880(buffer, sizeof(buffer)) == buffer);
    assert(std::memcmp(buffer, "1234567", 7) == 0);
    assert(buffer[7] == '\0');

    std::memset(buffer, 'X', sizeof(buffer));
    g_text = nullptr;
    assert(FUN_00401880(buffer, sizeof(buffer)) == nullptr);
    assert(buffer[0] == '\0');
    assert(buffer[7] == '\0');

    FUN_00401880_SetServices(nullptr);
    assert(FUN_00401880(buffer, sizeof(buffer)) == nullptr);
}
