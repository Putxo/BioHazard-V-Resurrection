#include "re5/recovered/fun_00402b10.hpp"

#include <cstring>

namespace re5::recovered {
namespace {
FUN_00402B10_Table* g_table = nullptr;
const FUN_00402B10_Services* g_services = nullptr;

const char* fallback_reset_key() noexcept {
    const auto* services = g_services;
    return services != nullptr && services->reset_key != nullptr ? services->reset_key : "";
}

bool key_matches_reset(const FUN_00402B10_Node& node) noexcept {
    const char* key = node.key_04 != nullptr ? node.key_04 : "";
    return std::strcmp(key, fallback_reset_key()) == 0;
}

bool accepted(const FUN_00402B10_Node& node) noexcept {
    const auto* services = g_services;
    if (services == nullptr || services->accept == nullptr) {
        return node.payload != nullptr;
    }
    return services->accept(services->context, node);
}

FUN_00402B10_Record make_record(const FUN_00402B10_Node& node) noexcept {
    const auto* services = g_services;
    if (services != nullptr && services->make_record != nullptr) {
        return services->make_record(services->context, node);
    }
    return FUN_00402B10_Record{node.key_04, node.payload, nullptr, nullptr};
}
}

void FUN_00402B10_SetServices(const FUN_00402B10_Services* services) noexcept {
    g_services = services;
}

void FUN_00402B10_SetTable(FUN_00402B10_Table* table) noexcept {
    g_table = table;
}

void FUN_00402B10(FUN_00402B10_Node* node) noexcept {
    if (node == nullptr || g_table == nullptr) {
        return;
    }

    for (FUN_00402B10_Node* current = node; current != nullptr; current = current->sibling_08) {
        if (key_matches_reset(*current)) {
            g_table->count = 0;
        } else if (g_table->count >= g_table->records.size()) {
            return;
        }

        if (current->child_0c != nullptr) {
            FUN_00402B10(current->child_0c);
        }

        if ((current->flags_1f & 1U) != 0U) {
            continue;
        }

        if (!accepted(*current)) {
            continue;
        }

        if (g_table->count >= g_table->records.size()) {
            return;
        }

        const std::uint32_t index = g_table->count;
        g_table->records[index] = FUN_00402B10_Record{};
        g_table->records[index] = make_record(*current);
        ++g_table->count;
        if (g_table->count >= g_table->records.size()) {
            return;
        }
    }
}

} // namespace re5::recovered
