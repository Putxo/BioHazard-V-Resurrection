#include "re5/recovered/fun_00405bd0.hpp"

#include <cstddef>

namespace re5::recovered {
namespace {
const FUN_00405BD0_Services* g_services = nullptr;

constexpr std::size_t k_local_path_size = 0x104U;
constexpr const char* k_archive_resource = "Image\\Archive\\MsgResource";
constexpr const char* k_archive_format = "%s\\%s.arc";
constexpr const char* k_empty_literal = "";
constexpr const char* k_special_key = "0x16E216C";
constexpr const char* k_message_key = "0x1692FC8";
constexpr const char* k_final_marker = "0x137AF90";

constexpr const char* k_message_paths[] = {
    "etc\\message\\mes_system",
    "etc\\message\\mes_item",
    "etc\\message\\mes_file",
    "etc\\message\\mes_event",
    "etc\\message\\mes_stage1",
    "etc\\message\\mes_stage2",
    "etc\\message\\mes_stage3",
    "etc\\message\\mes_stage5",
    "etc\\message\\mes_stage7",
};

void copy_text(char* destination, std::size_t capacity, const char* source) noexcept {
    if (destination == nullptr || capacity == 0U) {
        return;
    }
    const char* selected = source != nullptr ? source : k_empty_literal;
    std::size_t i = 0;
    while (selected[i] != '\0' && i + 1U < capacity) {
        destination[i] = selected[i];
        ++i;
    }
    destination[i] = '\0';
}

void build_archive_path(
    char* destination,
    std::size_t capacity,
    const char* root,
    const char* resource) noexcept {
    if (destination == nullptr || capacity == 0U) {
        return;
    }

    std::size_t out = 0;
    const char* root_text = root != nullptr ? root : k_empty_literal;
    const char* resource_text = resource != nullptr ? resource : k_empty_literal;
    for (std::size_t i = 0; root_text[i] != '\0' && out + 1U < capacity; ++i) {
        destination[out++] = root_text[i];
    }
    if (out + 1U < capacity) {
        destination[out++] = '\\';
    }
    for (std::size_t i = 0; resource_text[i] != '\0' && out + 1U < capacity; ++i) {
        destination[out++] = resource_text[i];
    }
    constexpr const char suffix[] = ".arc";
    for (std::size_t i = 0; suffix[i] != '\0' && out + 1U < capacity; ++i) {
        destination[out++] = suffix[i];
    }
    destination[out] = '\0';
}

const char* resolve_path(const FUN_00405BD0_Services& services, const char* path) noexcept {
    if (services.resolve_path == nullptr) {
        return path;
    }
    const char* resolved = services.resolve_path(services.context, path);
    return resolved != nullptr ? resolved : k_empty_literal;
}

bool can_bind(const FUN_00405BD0_Services& services, bool registered) noexcept {
    return !services.registry_suppressed_a &&
        !services.registry_suppressed_b &&
        registered &&
        services.bind_registered != nullptr;
}
} // namespace

void FUN_00405BD0_SetServices(const FUN_00405BD0_Services* services) noexcept {
    g_services = services;
}

void FUN_00405BD0(std::uint32_t message_index) noexcept {
    const auto* services = g_services;
    if (services == nullptr) {
        return;
    }

    const std::uint32_t previous_index = services->exchange_message_index != nullptr
        ? services->exchange_message_index(services->context, message_index)
        : message_index;
    const bool previous_loading = services->exchange_global_loading != nullptr
        ? services->exchange_global_loading(services->context, true)
        : false;

    char primary_path[k_local_path_size]{};
    char archive_path[k_local_path_size]{};
    copy_text(primary_path, sizeof(primary_path), resolve_path(*services, k_archive_resource));
    build_archive_path(
        archive_path,
        sizeof(archive_path),
        services->global_root != nullptr ? services->global_root : k_empty_literal,
        primary_path);

    bool skip_batch = false;
    if (services->archive_special_enabled) {
        if (services->prepare_archive != nullptr) {
            services->prepare_archive(services->context, archive_path);
        }

        bool registered = services->register_named != nullptr
            ? services->register_named(services->context, primary_path, k_special_key, true)
            : false;
        if (registered && services->finalize_registered != nullptr) {
            services->finalize_registered(services->context, &registered);
        }
        if (services->check_cancel != nullptr) {
            skip_batch = services->check_cancel(services->context, archive_path);
        }
    }

    if (!skip_batch) {
        if (services->begin_batch != nullptr) {
            services->begin_batch(services->context, primary_path);
        }
        if (services->start_batch != nullptr) {
            services->start_batch(services->context);
        }

        for (const char* path : k_message_paths) {
            const char* resolved = resolve_path(*services, path);
            bool registered = services->register_named != nullptr
                ? services->register_named(services->context, resolved, k_message_key, true)
                : false;
            if (can_bind(*services, registered)) {
                services->bind_registered(services->context, &registered);
            }
        }

        if (services->end_batch != nullptr) {
            services->end_batch(services->context, false);
        }
    }

    if (services->exchange_global_loading != nullptr) {
        (void)services->exchange_global_loading(services->context, previous_loading);
    }
    if (services->exchange_message_index != nullptr) {
        (void)services->exchange_message_index(services->context, previous_index);
    }
    if (services->log_marker != nullptr) {
        services->log_marker(services->context, k_final_marker);
    }

    (void)k_archive_format;
}

} // namespace re5::recovered
