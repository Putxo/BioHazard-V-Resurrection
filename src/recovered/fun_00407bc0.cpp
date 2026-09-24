#include "re5/recovered/fun_00407bc0.hpp"

namespace re5::recovered {
namespace {
const FUN_00407BC0_Services* g_services = nullptr;
} // namespace

void FUN_00407BC0_SetServices(const FUN_00407BC0_Services* services) noexcept {
    g_services = services;
}

bool FUN_00407BC0(
    FUN_00407BC0_Object& object,
    const FUN_00407BC0_Key& key) noexcept {
    const auto* services = g_services;
    if (services == nullptr || services->first_node == nullptr) {
        return false;
    }

    FUN_00407BC0_Node* node = services->first_node(services->context, &object);
    if (node == nullptr) {
        return false;
    }

    const std::uint32_t target = key.value;
    for (;;) {
        if (node->value == target) {
            return true;
        }

        node = node->next;
        if (node == nullptr) {
            return false;
        }
    }
}

} // namespace re5::recovered
