#include "re5/recovered/fun_00403640.hpp"

#include <cstdio>
#include <cstring>

namespace re5::recovered {
namespace {
const FUN_00403640_Services* g_services = nullptr;

const char* safe_text(const char* value, const FUN_00403640_Services& services) noexcept {
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

void trim_extension(char* text) noexcept {
    char* dot = std::strrchr(text, '.');
    if (dot != nullptr) {
        *dot = '\0';
    }
}
} // namespace

void FUN_00403640_SetServices(const FUN_00403640_Services* services) noexcept {
    g_services = services;
}

void FUN_00403640(void* object, const void* handle, const char* base_path, const char* key) noexcept {
    const auto* services = g_services;
    if (services == nullptr || services->validate_handle == nullptr || services->open_root == nullptr) {
        return;
    }
    if (!services->validate_handle(services->context, handle)) {
        return;
    }

    void* root = services->open_root(services->context, key, services->root_table_01652ef0);
    if (root == nullptr) {
        return;
    }

    if (services->is_registry_ready != nullptr &&
        !services->is_registry_ready(services->context, services->registry_root_016e20dc) &&
        services->ensure_registry != nullptr) {
        services->ensure_registry(services->context, services->registry_root_016e20dc);
    }

    if (services->next_entry != nullptr) {
        FUN_00403640_Entry entry{};
        while (services->next_entry(services->context, root, entry)) {
            const char* name = safe_text(entry.name, *services);
            if (dot_entry(name)) {
                continue;
            }

            char child_path[0x104]{};
            std::snprintf(
                child_path,
                sizeof(child_path),
                "%s/%s",
                safe_text(base_path, *services),
                name);

            if (entry.descend) {
                FUN_00403640(object, handle, child_path, key);
                continue;
            }

            char leaf[0x104]{};
            std::snprintf(leaf, sizeof(leaf), "%s", name);
            trim_extension(leaf);
            if (services->known_leaf != nullptr && !services->known_leaf(services->context, leaf)) {
                continue;
            }
            const void* value = services->lookup_leaf != nullptr
                ? services->lookup_leaf(services->context, leaf)
                : nullptr;
            if (value != nullptr && services->bind_leaf != nullptr) {
                services->bind_leaf(services->context, value, child_path);
            }
        }
    }

    if (services->close_root != nullptr) {
        services->close_root(services->context, root);
    }
}

} // namespace re5::recovered
