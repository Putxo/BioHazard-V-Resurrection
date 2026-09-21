#include "re5/recovered/fun_00405710.hpp"

#include <cstddef>

namespace re5::recovered {
namespace {
const FUN_00405710_Services* g_services = nullptr;

constexpr std::uint8_t k_skip_flag = 0x01U;
constexpr const char* k_primary_format = "0x137AAE0";
constexpr const char* k_secondary_format = "0x137AAD4";
constexpr const char* k_empty_literal = "0x137AA5C";
constexpr const char* k_special_key = "0x16E216C";
constexpr std::size_t k_local_path_size = 0x104U;
constexpr unsigned k_batch_attempts = 2U;

void default_format(const char* marker, const char* value, char* out, std::size_t out_size) noexcept {
    if (out == nullptr || out_size == 0U) {
        return;
    }
    const char* selected = value != nullptr ? value : "";
    std::size_t i = 0;
    for (; marker != nullptr && marker[i] != '\0' && i + 1U < out_size; ++i) {
        out[i] = marker[i];
    }
    if (i + 1U < out_size) {
        out[i++] = ':';
    }
    for (std::size_t j = 0; selected[j] != '\0' && i + 1U < out_size; ++j, ++i) {
        out[i] = selected[j];
    }
    out[i] = '\0';
}

void format_path(const FUN_00405710_Services& services, const char* marker, const char* value, char* out) noexcept {
    if (services.format_path != nullptr) {
        services.format_path(services.context, marker, value, out, k_local_path_size);
        return;
    }
    default_format(marker, value, out, k_local_path_size);
}

bool registry_allowed(const FUN_00405710_Services& services, bool registered) noexcept {
    return !services.registry_suppressed_a &&
        !services.registry_suppressed_b &&
        registered &&
        services.bind_registered != nullptr;
}
} // namespace

void FUN_00405710_SetServices(const FUN_00405710_Services* services) noexcept {
    g_services = services;
}

void FUN_00405710(FUN_00405710_Request& request) noexcept {
    const auto* services = g_services;
    if (services == nullptr) {
        return;
    }
    if ((request.flags_1f & k_skip_flag) != 0U) {
        return;
    }

    char primary_path[k_local_path_size]{};
    char secondary_path[k_local_path_size]{};
    format_path(*services, k_primary_format, request.name, primary_path);
    format_path(
        *services,
        k_secondary_format,
        services->global_root != nullptr ? services->global_root : k_empty_literal,
        secondary_path);

    if (services->global_special_enabled && services->register_named != nullptr) {
        (void)services->register_named(services->context, primary_path, k_special_key, true);
        if (services->check_cancel != nullptr && services->check_cancel(services->context, secondary_path)) {
            return;
        }
    }

    FUN_00405710_Child* child = services->open_child != nullptr
        ? services->open_child(services->context, request)
        : nullptr;
    if (child == nullptr) {
        return;
    }

    if (services->child_ready == nullptr || !services->child_ready(services->context, *child)) {
        if (services->release_child != nullptr) {
            services->release_child(services->context, *child, true);
        }
        return;
    }
    if (child->entries == nullptr || child->entry_count == 0U) {
        if (services->release_child != nullptr) {
            services->release_child(services->context, *child, true);
        }
        return;
    }

    if (services->set_global_loading != nullptr) {
        services->set_global_loading(services->context, true);
    }

    for (unsigned attempt = 0; attempt < k_batch_attempts; ++attempt) {
        char batch_path[k_local_path_size]{};
        if (services->begin_batch != nullptr) {
            services->begin_batch(services->context, batch_path, sizeof(batch_path));
        }

        for (std::size_t i = 0; i < child->entry_count; ++i) {
            const FUN_00405710_Entry& entry = child->entries[i];
            if (entry.name == nullptr || entry.name[0] == '\0') {
                break;
            }
            bool registered = services->register_named != nullptr
                ? services->register_named(services->context, entry.path, entry.name, true)
                : false;
            if (registry_allowed(*services, registered)) {
                services->bind_registered(services->context, &registered);
            }
        }

        if (services->end_batch != nullptr) {
            services->end_batch(services->context);
        }
        if (services->check_cancel != nullptr && services->check_cancel(services->context, batch_path)) {
            break;
        }
    }

    if (services->release_child != nullptr) {
        services->release_child(services->context, *child, true);
    }
    if (services->set_global_loading != nullptr) {
        services->set_global_loading(services->context, false);
    }
}

} // namespace re5::recovered
