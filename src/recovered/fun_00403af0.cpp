#include "re5/recovered/fun_00403af0.hpp"

#include <cstdio>
#include <cstring>

namespace re5::recovered {
namespace {
const FUN_00403AF0_Services* g_services = nullptr;

const char* safe_text(const char* text, const FUN_00403AF0_Services& services) noexcept {
    if (text != nullptr) {
        return text;
    }
    return services.fallback_name != nullptr ? services.fallback_name : "";
}

bool dot_entry(const char* name) noexcept {
    if (name == nullptr) {
        return false;
    }
    return std::strcmp(name, ".") == 0 || std::strcmp(name, "..") == 0;
}

void trim_extension(char* text) noexcept {
    char* dot = std::strrchr(text, '.');
    if (dot != nullptr) {
        *dot = '\0';
    }
}
} // namespace

void FUN_00403AF0_SetServices(const FUN_00403AF0_Services* services) noexcept {
    g_services = services;
}

void FUN_00403AF0(FUN_00403AF0_NameRef* name) noexcept {
    const auto* services = g_services;
    if (services == nullptr || services->validate_name == nullptr) {
        return;
    }

    const char* root_name = safe_text(name != nullptr ? name->text : nullptr, *services);
    if (!services->validate_name(services->context, root_name)) {
        return;
    }

    if (services->registry_ready != nullptr && !services->registry_ready(services->context) &&
        services->ensure_registry != nullptr) {
        services->ensure_registry(services->context, services->registry_root_016e20dc);
    }

    if (services->begin_scan == nullptr || !services->begin_scan(services->context, root_name)) {
        return;
    }

    if (services->next_entry != nullptr) {
        FUN_00403AF0_Entry entry{};
        while (services->next_entry(services->context, entry)) {
            const char* entry_name = safe_text(entry.name, *services);
            if (dot_entry(entry_name)) {
                continue;
            }

            if (entry.descend) {
                FUN_00403AF0_NameRef child{entry_name};
                FUN_00403AF0(&child);
                continue;
            }

            char leaf[0x104]{};
            std::snprintf(leaf, sizeof(leaf), "%s", entry_name);
            trim_extension(leaf);
            const FUN_00403AF0_TableRecord* record = services->find_record != nullptr
                ? services->find_record(services->context, leaf)
                : nullptr;
            if (record != nullptr && record->object != nullptr) {
                char path[0x104]{};
                std::snprintf(path, sizeof(path), "%s/%s", root_name, entry_name);
                if (services->bind_record != nullptr) {
                    services->bind_record(services->context, *record, path);
                }
            } else if (services->report_missing != nullptr) {
                services->report_missing(services->context, entry_name);
            }
        }
    }

    if (services->end_scan != nullptr) {
        services->end_scan(services->context);
    }
}

} // namespace re5::recovered
