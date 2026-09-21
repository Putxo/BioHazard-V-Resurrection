#include "re5/recovered/fun_00405a60.hpp"

#include <cstddef>
#include <cstdint>

namespace re5::recovered {
namespace {
const FUN_00405A60_Services* g_services = nullptr;

constexpr const char* k_begin_trace_marker = "0x137AFEC";
constexpr const char* k_end_trace_marker = "0x137AF90";
constexpr const char* k_single_format_marker = "0x137B518";
constexpr const char* k_archive_format_marker = "0x137AAD4";
constexpr const char* k_core_resource_path = "Image\\Archive\\Core2Resource";
constexpr std::uintptr_t k_special_key = 0x016E216CU;
constexpr std::size_t k_local_path_size = 0x104U;

struct StaticEntry {
    const char* path;
    std::uintptr_t key_token;
};

constexpr StaticEntry k_entries[] = {
    {"id\\jpn\\menu", 0x0165BB68U},
    {"etc\\message\\mes_tex", 0x016E21B4U},
    {"etc\\message\\mes_tex", 0x01692E3CU},
    {"etc\\message\\cursor_ps3_dc", 0x016E21B4U},
    {"etc\\message\\cursor_ps3_dc", 0x01692E3CU},
    {"etc\\vibration_dc", 0x016E22B0U},
};

void copy_text(const char* source, char* out, std::size_t out_size) noexcept {
    if (out == nullptr || out_size == 0U) {
        return;
    }
    const char* selected = source != nullptr ? source : "";
    std::size_t i = 0U;
    for (; selected[i] != '\0' && i + 1U < out_size; ++i) {
        out[i] = selected[i];
    }
    out[i] = '\0';
}

void format_default(
    const char* first,
    const char* second,
    char* out,
    std::size_t out_size) noexcept {
    if (second == nullptr) {
        copy_text(first, out, out_size);
        return;
    }
    if (out == nullptr || out_size == 0U) {
        return;
    }

    std::size_t used = 0U;
    const auto append = [&](const char* value) noexcept {
        const char* selected = value != nullptr ? value : "";
        for (std::size_t i = 0U; selected[i] != '\0' && used + 1U < out_size; ++i) {
            out[used++] = selected[i];
        }
    };
    append(first);
    if (used + 1U < out_size) {
        out[used++] = '\\';
    }
    append(second);
    append(".arc");
    out[used] = '\0';
}

void format_path(
    const FUN_00405A60_Services& services,
    const char* marker,
    const char* first,
    const char* second,
    char* out) noexcept {
    if (services.format_path != nullptr) {
        services.format_path(services.context, marker, first, second, out, k_local_path_size);
        return;
    }
    format_default(first, second, out, k_local_path_size);
}

void trace(const FUN_00405A60_Services& services, const char* marker) noexcept {
    if (services.trace_marker != nullptr) {
        services.trace_marker(services.context, marker);
    }
}

void* register_entry(
    const FUN_00405A60_Services& services,
    const char* path,
    std::uintptr_t key_token) noexcept {
    if (services.register_named == nullptr) {
        return nullptr;
    }
    return services.register_named(services.context, path, key_token, true);
}

bool binding_allowed(const FUN_00405A60_Services& services, const void* registered) noexcept {
    return !services.registry_suppressed_a &&
        !services.registry_suppressed_b &&
        registered != nullptr &&
        services.bind_registered != nullptr;
}
} // namespace

void FUN_00405A60_SetServices(const FUN_00405A60_Services* services) noexcept {
    g_services = services;
}

void FUN_00405A60() noexcept {
    const auto* services = g_services;
    if (services == nullptr) {
        return;
    }

    trace(*services, k_begin_trace_marker);

    char primary_path[k_local_path_size]{};
    char secondary_path[k_local_path_size]{};
    format_path(*services, k_single_format_marker, k_core_resource_path, nullptr, primary_path);
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

    for (const auto& entry : k_entries) {
        void* registered = register_entry(*services, entry.path, entry.key_token);
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
}

} // namespace re5::recovered
