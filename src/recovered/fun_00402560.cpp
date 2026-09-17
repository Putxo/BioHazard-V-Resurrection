#include "re5/recovered/fun_00402560.hpp"

#include <cstdio>
#include <cstring>

namespace re5::recovered {
namespace {
const FUN_00402640_Table* g_table = nullptr;
const FUN_00402770_Services* g_services_00402770 = nullptr;
const FUN_00402DF0_Services* g_services_00402df0 = nullptr;
const FUN_00402E70_Services* g_services_00402e70 = nullptr;

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

void FUN_00402770_SetServices(const FUN_00402770_Services* services) noexcept {
    g_services_00402770 = services;
}

void FUN_00402DF0_SetServices(const FUN_00402DF0_Services* services) noexcept {
    g_services_00402df0 = services;
}

void FUN_00402E70_SetServices(const FUN_00402E70_Services* services) noexcept {
    g_services_00402e70 = services;
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

const void* FUN_004026C0(const FUN_00402640_StringRef& value) noexcept {
    const auto* table = g_table;
    if (table == nullptr || table->names == nullptr || table->values == nullptr) {
        return nullptr;
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
            return table->values[index];
        }
    }
    return nullptr;
}

void FUN_00402770(const char* text) noexcept {
    if (text == nullptr) {
        return;
    }

    const auto* services = g_services_00402770;
    if (services == nullptr || services->submit == nullptr) {
        return;
    }

    const std::size_t length = std::strlen(text);
    const std::uint32_t width_x16 = static_cast<std::uint32_t>((0x50U - length) << 4U);
    FUN_00402770_Request request{
        text,
        width_x16,
        width_x16 >> 1U,
        0x10U,
        0x10U,
        0x15EU,
        services->resource_0165201c,
        services->resource_01652e48,
    };
    services->submit(services->context, request);
}

void FUN_00402DF0(FUN_00402DF0_Object& object, std::int32_t selector) noexcept {
    const auto* services = g_services_00402df0;
    const char* name_format = "%d";
    const char* message_format = "%s %s";
    const char* active_name = "";

    if (services != nullptr) {
        if (services->name_format != nullptr) {
            name_format = services->name_format;
        }
        if (services->message_format != nullptr) {
            message_format = services->message_format;
        }
        active_name = services->fallback_active_name != nullptr ? services->fallback_active_name : "";
    }

    std::snprintf(object.name_54, sizeof(object.name_54), name_format, selector);

    if (services != nullptr && services->active_name != nullptr) {
        const char* provided = services->active_name(services->context);
        if (provided != nullptr) {
            active_name = provided;
        }
    }

    FUN_00402DF0_Message message{};
    message.formatted_name = object.name_54;
    message.active_name = active_name;
    std::snprintf(message.text, sizeof(message.text), message_format, active_name, object.name_54);

    if (services != nullptr && services->report != nullptr) {
        services->report(services->context, message);
    }

    if (services != nullptr && services->resolve != nullptr) {
        void* resolved = services->resolve(
            services->context,
            services->registry_016e216c,
            object.name_54,
            true);
        if (resolved != nullptr && services->activate != nullptr) {
            services->activate(services->context, resolved);
        }
    }
}

bool FUN_00402E70(FUN_00402DF0_Object& object, std::int32_t filter) noexcept {
    const auto* services = g_services_00402e70;

    if (object.state_2c == 2U) {
        if (services != nullptr && services->release_first != nullptr) {
            services->release_first(services->context, object.selected_50);
        }
        if (services != nullptr && services->release_second != nullptr) {
            services->release_second(services->context, object.selected_50);
        }
        if (object.current_4c != nullptr) {
            object.current_4c = object.current_4c->next_08;
        }
        --object.state_2c;
        return false;
    }

    if (object.state_2c == 0U) {
        object.current_4c = services != nullptr ? services->list_head_01657f20 : nullptr;
        if (filter < 0) {
            while (object.current_4c != nullptr) {
                const char* name = object.current_4c->name_04 != nullptr ? object.current_4c->name_04 : "";
                if (name[2] > '0') {
                    break;
                }
                object.current_4c = object.current_4c->next_08;
            }
        }
        object.state_2c = 1U;
    } else if (object.state_2c != 1U) {
        return false;
    }

    if (object.current_4c == nullptr) {
        return true;
    }

    const char* name = object.current_4c->name_04 != nullptr ? object.current_4c->name_04 : "";
    if (filter >= 0 && name[2] != static_cast<char>(filter + '0')) {
        object.current_4c = object.current_4c->next_08;
        return false;
    }

    const auto digit = [](char value) noexcept -> std::int32_t {
        return value >= '0' && value <= '9' ? static_cast<std::int32_t>(value - '0') : 0;
    };
    object.selected_50 = digit(name[2]) * 100 + digit(name[3]) * 10 + digit(name[4]);

    char label[0x100]{};
    const char* label_format = services != nullptr && services->label_format != nullptr
        ? services->label_format
        : "%d";
    std::snprintf(label, sizeof(label), label_format, object.selected_50);
    if (services != nullptr && services->submit_label != nullptr) {
        services->submit_label(services->context, label, object.selected_50);
    } else {
        FUN_00402770(label);
    }

    ++object.state_2c;
    return false;
}

} // namespace re5::recovered
