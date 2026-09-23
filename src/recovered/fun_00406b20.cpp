#include "re5/recovered/fun_00406b20.hpp"

#include <cstddef>
#include <cstdint>

namespace re5::recovered {
namespace {
const FUN_00406B20_Services* g_services = nullptr;

constexpr const char* k_begin_trace_marker = "0x137AFEC";
constexpr const char* k_end_trace_marker = "0x137AF90";
constexpr const char* k_single_format_marker = "0x137B518";
constexpr const char* k_archive_format_marker = "0x137AAD4";
constexpr const char* k_load_resource_path = "Image\\Archive\\LoadResource";
constexpr std::uintptr_t k_special_key = 0x016E216CU;
constexpr std::uintptr_t k_entry_key = 0x01692E3CU;
constexpr std::size_t k_local_path_size = 0x104U;

constexpr const char* k_entries[] = {
    "id\\jpn\\cockpit\\cp_load_360",
    "id\\eng\\cockpit\\cp_load_360",
    "id\\ger\\cockpit\\cp_load_360",
    "id\\fra\\cockpit\\cp_load_360",
    "id\\spa\\cockpit\\cp_load_360",
    "id\\ita\\cockpit\\cp_load_360",
    "id\\rus\\cockpit\\cp_load_360",
    "id\\pol\\cockpit\\cp_load_360",
    "id\\cze\\cockpit\\cp_load_360",
    "id\\chT\\cockpit\\cp_load_360",
    "id\\chS\\cockpit\\cp_load_360",
    "id\\jpn\\show_E3\\e3_load",
    "id\\eng\\show_E3\\e3_load",
};

void copy_text(const char* source, char* out, std::size_t out_size) noexcept {
    if (out == nullptr || out_size == 0U) return;
    const char* selected = source != nullptr ? source : "";
    std::size_t i = 0U;
    for (; selected[i] != '\0' && i + 1U < out_size; ++i) out[i] = selected[i];
    out[i] = '\0';
}

void format_default(const char* first, const char* second, char* out, std::size_t out_size) noexcept {
    if (second == nullptr) {
        copy_text(first, out, out_size);
        return;
    }
    if (out == nullptr || out_size == 0U) return;
    std::size_t used = 0U;
    const auto append = [&](const char* value) noexcept {
        const char* selected = value != nullptr ? value : "";
        for (std::size_t i = 0U; selected[i] != '\0' && used + 1U < out_size; ++i) out[used++] = selected[i];
    };
    append(first);
    if (used + 1U < out_size) out[used++] = '\\';
    append(second);
    append(".arc");
    out[used] = '\0';
}

void format_path(const FUN_00406B20_Services& s, const char* marker, const char* first, const char* second, char* out) noexcept {
    if (s.format_path != nullptr) {
        s.format_path(s.context, marker, first, second, out, k_local_path_size);
        return;
    }
    format_default(first, second, out, k_local_path_size);
}

void trace(const FUN_00406B20_Services& s, const char* marker) noexcept {
    if (s.trace_marker != nullptr) s.trace_marker(s.context, marker);
}

void* register_entry(const FUN_00406B20_Services& s, const char* path, std::uintptr_t key) noexcept {
    if (s.register_named == nullptr) return nullptr;
    return s.register_named(s.context, path, key, true);
}

bool binding_allowed(const FUN_00406B20_Services& s, const void* registered) noexcept {
    return !s.registry_suppressed_a && !s.registry_suppressed_b &&
        registered != nullptr && s.bind_registered != nullptr;
}
} // namespace

void FUN_00406B20_SetServices(const FUN_00406B20_Services* services) noexcept {
    g_services = services;
}

void FUN_00406B20() noexcept {
    const auto* services = g_services;
    if (services == nullptr) return;

    trace(*services, k_begin_trace_marker);

    char primary_path[k_local_path_size]{};
    char secondary_path[k_local_path_size]{};
    format_path(*services, k_single_format_marker, k_load_resource_path, nullptr, primary_path);
    format_path(
        *services,
        k_archive_format_marker,
        services->global_root != nullptr ? services->global_root : "",
        primary_path,
        secondary_path);

    if (services->global_special_enabled) {
        if (services->prepare_secondary != nullptr) {
            services->prepare_secondary(services->context, secondary_path);
        }
        void* registered = register_entry(*services, primary_path, k_special_key);
        if (registered != nullptr && services->finalize_registered != nullptr) {
            services->finalize_registered(services->context, registered);
        }
        if (services->check_cancel != nullptr && services->check_cancel(services->context, secondary_path)) {
            return;
        }
    }

    const std::uint8_t previous_loading = services->get_global_loading != nullptr
        ? services->get_global_loading(services->context)
        : 0U;
    if (services->set_global_loading != nullptr) {
        services->set_global_loading(services->context, 1U);
    }

    if (services->begin_batch != nullptr) {
        services->begin_batch(services->context, primary_path);
    }
    if (services->start_batch != nullptr) {
        services->start_batch(services->context);
    }

    for (const auto* path : k_entries) {
        void* registered = register_entry(*services, path, k_entry_key);
        if (binding_allowed(*services, registered)) {
            void* registered_slot = registered;
            services->bind_registered(services->context, &registered_slot);
        }
    }

    if (services->end_batch != nullptr) {
        services->end_batch(services->context, false);
    }
    if (services->set_global_loading != nullptr) {
        services->set_global_loading(services->context, previous_loading);
    }

    trace(*services, k_end_trace_marker);

    if (services->load_message_group != nullptr) {
        for (std::uint32_t index = 0U; index < 11U; ++index) {
            services->load_message_group(services->context, index);
        }
    }
}

} // namespace re5::recovered
