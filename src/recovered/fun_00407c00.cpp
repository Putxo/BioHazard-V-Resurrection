#include "re5/recovered/fun_00407c00.hpp"

namespace re5::recovered {
namespace {
const FUN_00407C00_Services* g_services = nullptr;
} // namespace

void FUN_00407C00_SetServices(const FUN_00407C00_Services* services) noexcept {
    g_services = services;
}

FUN_00407C00_Object* FUN_00407C00(
    FUN_00407C00_Object& object,
    const FUN_00407C00_Key& key) noexcept {
    const auto* services = g_services;
    if (services == nullptr || services->first_node == nullptr) {
        return nullptr;
    }

    FUN_00407C00_Node* node = services->first_node(services->context, &object);
    if (node == nullptr) {
        return nullptr;
    }

    const std::uint32_t target = key.value;
    for (;;) {
        if (node->value == target) {
            return &object;
        }

        node = node->next;
        if (node == nullptr) {
            return nullptr;
        }
    }
}

} // namespace re5::recovered
