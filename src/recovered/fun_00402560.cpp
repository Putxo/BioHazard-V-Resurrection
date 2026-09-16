#include "re5/recovered/fun_00402560.hpp"

#include <cstring>

namespace re5::recovered {
namespace {
const FUN_00402640_Table* g_table = nullptr;

const char* payload_string(const FUN_00402640_StringRef& value) noexcept {
    if (value.pointer == nullptr) {
        const auto* table = g_table;
        return table != nullptr ? table->fallback_literal : "";
    }

    const auto* bytes = static_cast<const char*>(value.pointer);
    return bytes + 8;
}
}

void FUN_00402640_SetTable(const FUN_00402640_Table* table) noexcept {
    g_table = table;
}

std::uint32_t FUN_00402560(
    FUN_00402560_Context& context,
    const FUN_00402560_Node* node,
    std::int32_t depth) noexcept {
    std::uint32_t count = 0;
    for (const FUN_00402560_Node* current = node; current != nullptr;) {
        if (current->child_0c != nullptr) {
            count += FUN_00402560(context, current->child_0c, depth + 1);
        }

        if ((current->flags_1f & 1U) == 0U) {
            ++count;
            if (context.visit != nullptr) {
                context.visit(context.visit_context, current, depth);
            }
        }

        if (depth == 0) {
            break;
        }
        current = current->sibling_08;
    }
    return count;
}

void FUN_004025D0(FUN_00402560_Context& context, FUN_00402560_Node* node, std::int32_t depth) noexcept {
    for (FUN_00402560_Node* current = node; current != nullptr;) {
        if (current->child_0c != nullptr) {
            FUN_004025D0(context, current->child_0c, depth + 1);
        }

        if ((current->flags_1f & 1U) == 0U && context.output_34 != nullptr) {
            context.output_34[context.output_count_38] = current;
            ++context.output_count_38;
        }

        if (depth == 0) {
            break;
        }
        current = current->sibling_08;
    }
}

bool FUN_00402640(const FUN_00402640_StringRef& value) noexcept {
    const auto* table = g_table;
    if (table == nullptr || table->names == nullptr) {
        return false;
    }

    const char* target = payload_string(value);
    if (target == nullptr) {
        target = "";
    }

    for (std::uint32_t index = 0; index < table->count; ++index) {
        const char* name = table->names[index];
        if (name == nullptr) {
            name = "";
        }
        if (std::strcmp(name, target) == 0) {
            return true;
        }
    }
    return false;
}

} // namespace re5::recovered
