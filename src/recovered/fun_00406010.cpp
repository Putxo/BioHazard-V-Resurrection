#include "re5/recovered/fun_00406010.hpp"

#include <cstdio>
#include <cstring>

namespace re5::recovered {
namespace {
const FUN_00406010_Services* g_services = nullptr;

const char* safe_text(const char* value, const FUN_00406010_Services& services) noexcept {
    if (value != nullptr) {
        return value;
    }
    return services.fallback_name != nullptr ? services.fallback_name : "";
}

bool dot_entry(const char* name) noexcept {
    if (name == nullptr) {
        return false;
    }
    return std::strcmp(name, ".") == 0 || std::strcmp(name, "..") == 0;
}

bool format_wildcard(char (&out)[0x104], const char* active_root, const char* base_path) noexcept {
    const int written = std::snprintf(out, sizeof(out), "%s\\%s\\*", active_root, base_path);
    return written >= 0 && static_cast<std::size_t>(written) < sizeof(out);
}

bool format_child(char (&out)[0x104], const char* base_path, const char* name) noexcept {
    const int written = std::snprintf(out, sizeof(out), "%s\\%s", base_path, name);
    return written >= 0 && static_cast<std::size_t>(written) < sizeof(out);
}
} // namespace

void FUN_00406010_SetServices(const FUN_00406010_Services* services) noexcept {
    g_services = services;
}

void FUN_00406010(const char* base_path, const char* key, const char* extension) noexcept {
    const auto* services = g_services;
    if (services == nullptr || services->open_type == nullptr) {
        return;
    }

    void* type_root = services->open_type(services->context, key, services->root_table_01652ef0);
    if (type_root == nullptr) {
        return;
    }

    const char* active_root = safe_text(services->active_root, *services);
    const char* base = safe_text(base_path, *services);
    char wildcard[0x104]{};
    if (!format_wildcard(wildcard, active_root, base) && services->report_path_error != nullptr) {
        services->report_path_error(services->context, active_root, base);
    }

    if (services->begin_scan == nullptr) {
        return;
    }
    void* scan = services->begin_scan(services->context, wildcard);
    if (scan == nullptr) {
        return;
    }

    if (services->next_entry != nullptr) {
        FUN_00406010_Entry entry{};
        while (services->next_entry(services->context, scan, entry)) {
            const char* name = safe_text(entry.name, *services);
            if (dot_entry(name)) {
                continue;
            }

            char child_path[0x104]{};
            if (!format_child(child_path, base, name) && services->report_path_error != nullptr) {
                services->report_path_error(services->context, base, name);
            }

            if (entry.directory) {
                FUN_00406010(child_path, key, extension);
                continue;
            }

            if (extension == nullptr) {
                continue;
            }
            char* match = std::strstr(child_path, extension);
            if (match == nullptr) {
                continue;
            }
            *match = '\0';

            void* resolved = services->resolve_leaf != nullptr
                ? services->resolve_leaf(services->context, type_root, child_path, true)
                : nullptr;
            if (!services->registry_suppressed_a && !services->registry_suppressed_b &&
                resolved != nullptr && services->register_resolved != nullptr) {
                services->register_resolved(services->context, resolved);
            }
        }
    }

    if (services->end_scan != nullptr) {
        services->end_scan(services->context, scan);
    }
}

} // namespace re5::recovered
