#include "re5/recovered/fun_00401880.hpp"

#include <cstring>

namespace re5::recovered {
namespace {
const FUN_00401880_Services* g_services = nullptr;
}

void FUN_00401880_SetServices(const FUN_00401880_Services* services) noexcept {
    g_services = services;
}

char* FUN_00401880(char* destination, std::uint32_t capacity) noexcept {
    const auto* services = g_services;
    if (services == nullptr || services->get_friends == nullptr || services->invoke_slot_00 == nullptr || destination == nullptr || capacity == 0U) {
        return nullptr;
    }

    void* friends = services->get_friends(services->context);
    if (friends == nullptr) {
        destination[0] = '\0';
        destination[capacity - 1U] = '\0';
        return nullptr;
    }

    const char* text = services->invoke_slot_00(services->context, friends);
    destination[0] = '\0';
    if (text == nullptr) {
        destination[capacity - 1U] = '\0';
        return nullptr;
    }

    std::strncpy(destination, text, static_cast<std::size_t>(capacity - 1U));
    destination[capacity - 1U] = '\0';
    return destination;
}

} // namespace re5::recovered
