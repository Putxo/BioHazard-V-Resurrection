#include "re5/recovered/fun_00403150.hpp"

#include <cstdio>
#include <cstring>

namespace re5::recovered {
namespace {
const FUN_00403150_Services* g_services = nullptr;

const char* fallback_name(const FUN_00403150_Services& services) noexcept {
    return services.fallback_name != nullptr ? services.fallback_name : "";
}

bool dot_entry(const char* name) noexcept {
    if (name == nullptr) {
        return false;
    }
    return std::strcmp(name, ".") == 0 || std::strcmp(name, "..") == 0;
}

void trim_extension(char* name) noexcept {
    char* dot = std::strrchr(name, '.');
    if (dot != nullptr) {
        *dot = '\0';
    }
}
} // namespace

void FUN_00403150_SetServices(const FUN_00403150_Services* services) noexcept {
    g_services = services;
}

void FUN_00403150(const char* base_path, const char* key, void* root_context) noexcept {
    const auto* services = g_services;
    if (services == nullptr || services->open_root == nullptr) {
        return;
    }

    void* root = services->open_root(services->context, key, services->root_table_01652ef0);
    if (root == nullptr) {
        return;
    }

    if (services->begin_scan != nullptr) {
        services->begin_scan(services->context, root);
    }

    const char* active = services->active_name != nullptr ? services->active_name : fallback_name(*services);
    const char* header_format = services->header_format != nullptr ? services->header_format : "%s %s";
    char header[0x104]{};
    if (std::snprintf(header, sizeof(header), header_format, active, key != nullptr ? key : "") < 0) {
        if (services->report_error != nullptr) {
            const char* error_format = services->error_format != nullptr ? services->error_format : "%s %s";
            char error[0x104]{};
            std::snprintf(error, sizeof(error), error_format, active, key != nullptr ? key : "");
            services->report_error(services->context, error);
        }
    }

    if (services->next_entry != nullptr) {
        FUN_00403150_Entry entry{};
        while (services->next_entry(services->context, root, entry)) {
            const char* entry_name = entry.name != nullptr ? entry.name : fallback_name(*services);
            if (!dot_entry(entry_name)) {
                char child_path[0x104]{};
                const char* child_format = services->child_format != nullptr ? services->child_format : "%s/%s";
                std::snprintf(
                    child_path,
                    sizeof(child_path),
                    child_format,
                    base_path != nullptr ? base_path : "",
                    entry_name);

                if (entry.descend) {
                    FUN_00403150(child_path, key, root_context);
                } else if (services->resolve_leaf != nullptr) {
                    char leaf[0x104]{};
                    const char* leaf_format = services->leaf_format != nullptr ? services->leaf_format : "%s";
                    std::snprintf(leaf, sizeof(leaf), leaf_format, child_path);
                    trim_extension(leaf);
                    void* resolved = services->resolve_leaf(services->context, root_context, leaf, true);
                    if (resolved != nullptr && services->activate != nullptr) {
                        services->activate(services->context, resolved);
                    }
                }
            }

            if (services->end_scan != nullptr && !services->end_scan(services->context, root)) {
                break;
            }
        }
    }

    if (services->close_root != nullptr) {
        services->close_root(services->context, root);
    }
}

} // namespace re5::recovered
