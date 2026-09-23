#include "re5/recovered/fun_00406910.hpp"

#include <cstdio>

namespace re5::recovered {
namespace {
const FUN_00406910_Services* g_services = nullptr;

constexpr std::int32_t k_first_stage = 0x190;
constexpr std::int32_t k_stage_count = 8;

constexpr const char* k_trace_begin =
    ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
constexpr const char* k_trace_end =
    "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";
constexpr const char* k_archive_pattern = "Image\\Archive\\s%03dSub";
constexpr const char* k_archive_path_pattern = "%s\\%s.arc";
constexpr const char* k_stage_path_pattern = "stage\\s%03d\\soft\\mercedc";
constexpr const char* k_sound_resource = "sound\\bgm_dc\\s4xx_radio";

constexpr std::uintptr_t k_archive_registry = 0x016E216CU;
constexpr std::uintptr_t k_sound_registry = 0x016E2830U;

bool valid_stage(std::int32_t stage_id) noexcept {
    return static_cast<std::uint32_t>(stage_id - k_first_stage) <
        static_cast<std::uint32_t>(k_stage_count);
}

bool services_complete(const FUN_00406910_Services& s) noexcept {
    return s.trace_marker != nullptr &&
        s.get_convert_mode != nullptr &&
        s.set_convert_mode != nullptr &&
        s.resource_root != nullptr &&
        s.special_mode_enabled != nullptr &&
        s.prepare_archive != nullptr &&
        s.resolve_resource != nullptr &&
        s.activate_resource != nullptr &&
        s.archive_ready != nullptr &&
        s.convert_archive != nullptr &&
        s.flush_archive_conversion != nullptr &&
        s.registry_suppressed_a != nullptr &&
        s.registry_suppressed_b != nullptr &&
        s.register_resolved != nullptr &&
        s.convert_directory != nullptr &&
        s.finish_conversion != nullptr;
}
} // namespace

void FUN_00406910_SetServices(const FUN_00406910_Services* services) noexcept {
    g_services = services;
}

void FUN_00406910(std::int32_t stage_id) noexcept {
    // Native guard: only stage ids 0x190..0x197 enter the conversion body.
    if (!valid_stage(stage_id)) {
        return;
    }

    const auto* services = g_services;
    if (services == nullptr || !services_complete(*services)) {
        return;
    }

    char archive_name[0x100]{};
    char stage_path[0x100]{};
    char archive_path[0x100]{};

    std::snprintf(archive_name, sizeof(archive_name), k_archive_pattern, stage_id);

    services->trace_marker(services->context, k_trace_begin);

    const std::uint8_t saved_mode = services->get_convert_mode(services->context);
    services->set_convert_mode(services->context, 1U);

    const char* root = services->resource_root(services->context);
    if (root == nullptr) {
        root = "";
    }
    std::snprintf(
        archive_path,
        sizeof(archive_path),
        k_archive_path_pattern,
        root,
        archive_name);

    if (services->special_mode_enabled(services->context)) {
        services->prepare_archive(services->context, archive_path);

        void* archive = services->resolve_resource(
            services->context,
            k_archive_registry,
            archive_name,
            true);
        if (archive != nullptr) {
            services->activate_resource(services->context, archive);
        }

        if (services->archive_ready(services->context, archive_path)) {
            services->set_convert_mode(services->context, saved_mode);
            services->trace_marker(services->context, k_trace_end);
            return;
        }
    }

    services->convert_archive(services->context, archive_name);
    services->flush_archive_conversion(services->context);

    void* sound = services->resolve_resource(
        services->context,
        k_sound_registry,
        k_sound_resource,
        true);

    if (!services->registry_suppressed_a(services->context) &&
        !services->registry_suppressed_b(services->context) &&
        sound != nullptr) {
        services->register_resolved(services->context, sound);
    }

    std::snprintf(stage_path, sizeof(stage_path), k_stage_path_pattern, stage_id);
    services->convert_directory(
        services->context,
        stage_path,
        "rLayout",
        ".lot");
    services->convert_directory(
        services->context,
        stage_path,
        "rScenario",
        ".sce");
    services->convert_directory(
        services->context,
        stage_path,
        "rAIFSM",
        ".fsm");

    services->finish_conversion(services->context, 0);

    services->set_convert_mode(services->context, saved_mode);
    services->trace_marker(services->context, k_trace_end);
}

} // namespace re5::recovered
