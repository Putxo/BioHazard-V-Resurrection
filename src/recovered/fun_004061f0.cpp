#include "re5/recovered/fun_004061f0.hpp"

#include <cstddef>

namespace re5::recovered {
namespace {
const FUN_004061F0_Services* g_services = nullptr;

constexpr std::size_t k_local_path_size = 0x104U;
constexpr std::uint32_t k_special_key_address = 0x016E216CU;
constexpr const char* k_empty_literal = "";
constexpr const char* k_start_marker = "0x0137AFEC";
constexpr const char* k_final_marker = "0x0137AF90";

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

    const char* root_text = root != nullptr ? root : k_empty_literal;
    const char* resource_text = resource != nullptr ? resource : k_empty_literal;
    std::size_t out = 0;

    for (std::size_t i = 0; root_text[i] != '\0' && out + 1U < capacity; ++i) {
        destination[out++] = root_text[i];
    }
    if (out + 1U < capacity) {
        destination[out++] = '\\';
    }
    for (std::size_t i = 0; resource_text[i] != '\0' && out + 1U < capacity; ++i) {
        destination[out++] = resource_text[i];
    }

    constexpr char suffix[] = ".arc";
    for (std::size_t i = 0; suffix[i] != '\0' && out + 1U < capacity; ++i) {
        destination[out++] = suffix[i];
    }
    destination[out] = '\0';
}

const char* resolve_indexed(
    const FUN_004061F0_Services& services,
    const char* path) noexcept {
    if (services.resolve_indexed_path == nullptr) {
        return path != nullptr ? path : k_empty_literal;
    }
    const char* resolved = services.resolve_indexed_path(services.context, path);
    return resolved != nullptr ? resolved : k_empty_literal;
}

const char* resolve_table(
    const FUN_004061F0_Services& services,
    const char* path) noexcept {
    if (services.resolve_table_path == nullptr) {
        return path != nullptr ? path : k_empty_literal;
    }
    const char* resolved = services.resolve_table_path(services.context, path);
    return resolved != nullptr ? resolved : k_empty_literal;
}

void bind_if_allowed(
    const FUN_004061F0_Services& services,
    void*& registered) noexcept {
    if (!services.registry_suppressed_a &&
        !services.registry_suppressed_b &&
        registered != nullptr &&
        services.bind_registered != nullptr) {
        services.bind_registered(services.context, &registered);
    }
}

void register_direct_entries(
    const FUN_004061F0_Services& services,
    const FUN_004061F0_Entry* entries,
    std::int32_t count) noexcept {
    if (entries == nullptr || count <= 0) {
        return;
    }

    for (std::int32_t i = 0; i < count; ++i) {
        void* registered = services.register_named != nullptr
            ? services.register_named(
                services.context,
                entries[i].path != nullptr ? entries[i].path : k_empty_literal,
                entries[i].key_address,
                true)
            : nullptr;
        bind_if_allowed(services, registered);
    }
}

void register_resolved_entries(
    const FUN_004061F0_Services& services,
    const FUN_004061F0_Entry* entries,
    std::int32_t count) noexcept {
    if (entries == nullptr || count <= 0) {
        return;
    }

    for (std::int32_t i = 0; i < count; ++i) {
        const char* path = resolve_table(services, entries[i].path);
        void* registered = services.register_named != nullptr
            ? services.register_named(
                services.context,
                path,
                entries[i].key_address,
                true)
            : nullptr;
        bind_if_allowed(services, registered);
    }
}
} // namespace

void FUN_004061F0_SetServices(const FUN_004061F0_Services* services) noexcept {
    g_services = services;
}

void FUN_004061F0(
    std::int32_t message_index,
    const char* archive_resource,
    const FUN_004061F0_Entry* direct_entries,
    std::int32_t direct_count,
    const FUN_004061F0_Entry* resolved_entries,
    std::int32_t resolved_count) noexcept {
    const auto* services = g_services;
    if (services == nullptr) {
        return;
    }

    if (services->log_marker != nullptr) {
        services->log_marker(services->context, k_start_marker);
    }

    const bool have_saved_index = services->read_message_index != nullptr;
    const std::int32_t previous_index = have_saved_index
        ? services->read_message_index(services->context)
        : message_index;

    if (message_index >= 0 && services->write_message_index != nullptr) {
        services->write_message_index(services->context, message_index);
    }

    const bool previous_loading = services->exchange_global_loading != nullptr
        ? services->exchange_global_loading(services->context, true)
        : false;

    char primary_path[k_local_path_size]{};
    char archive_path[k_local_path_size]{};
    const char* selected_primary = message_index >= 0
        ? resolve_indexed(*services, archive_resource)
        : (archive_resource != nullptr ? archive_resource : k_empty_literal);
    copy_text(primary_path, sizeof(primary_path), selected_primary);
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

        void* registered = services->register_named != nullptr
            ? services->register_named(
                services->context,
                primary_path,
                k_special_key_address,
                true)
            : nullptr;
        if (registered != nullptr && services->finalize_registered != nullptr) {
            services->finalize_registered(services->context, registered);
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

        register_direct_entries(*services, direct_entries, direct_count);
        register_resolved_entries(*services, resolved_entries, resolved_count);

        if (services->end_batch != nullptr) {
            services->end_batch(services->context, false);
        }
    }

    if (services->exchange_global_loading != nullptr) {
        (void)services->exchange_global_loading(services->context, previous_loading);
    }
    if (have_saved_index && services->write_message_index != nullptr) {
        services->write_message_index(services->context, previous_index);
    }
    if (services->log_marker != nullptr) {
        services->log_marker(services->context, k_final_marker);
    }
}

} // namespace re5::recovered
