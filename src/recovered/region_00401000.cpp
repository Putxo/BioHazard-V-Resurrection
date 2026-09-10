#include "re5/recovered/region_00401000.hpp"
#include "re5/runtime/host_runtime.hpp"

namespace re5::recovered {
namespace {
constexpr std::size_t kBuildTitleCapacity = 0x104U;
constexpr char16_t kWindowClass[] = u"MTFramework";
constexpr char kBuildNumber[] = "3170";
constexpr char kBuildConfig[] = "RELEASE";
constexpr char kBuildTimestamp[] = "Thu Apr 16, 2015 06:51:32 PM";
constexpr char kCheatsMarker[] = "(CHEATS)";
constexpr std::uint32_t kAchievementCount = 0x46U;
constexpr std::uint32_t kPrimaryUserIndex = 0U;
constexpr std::uint32_t kSigninInfoFlags = 1U;
constexpr std::uint32_t kSignedInToLive = 2U;
constexpr std::uint32_t kResidentEvil5TitleId = 0x434307F7U;
constexpr std::uint32_t kDefaultEnumerationCount = 0xFFU;
constexpr std::uint32_t kInvalidEnumerationHandle = 0xFFFFFFFFU;
constexpr std::uintptr_t kFUN_00401410Key = 0x0137A6ACU;
constexpr std::uint32_t kFUN_00401410Threshold = 0x64U;
constexpr std::uint32_t kFUN_00401470SuccessCode = 2U;

const FUN_00401270_Services* g_FUN_00401270_services = nullptr;
const FUN_004012F0_Services* g_FUN_004012F0_services = nullptr;
const FUN_00401410_Services* g_FUN_00401410_services = nullptr;
const FUN_00401470_Services* g_FUN_00401470_services = nullptr;

void append_ascii(char16_t* out, std::size_t& pos, const char* text) noexcept {
    if (out == nullptr || text == nullptr) {
        return;
    }
    while (*text != '\0' && pos + 1U < kBuildTitleCapacity) {
        out[pos++] = static_cast<unsigned char>(*text++);
    }
}

void append_wide_literal(char16_t* out, std::size_t& pos, const char16_t* text) noexcept {
    if (out == nullptr || text == nullptr) {
        return;
    }
    while (*text != u'\0' && pos + 1U < kBuildTitleCapacity) {
        out[pos++] = *text++;
    }
}
} // namespace

void FUN_00401000(void* ptr) noexcept {
    re5::runtime::aligned_release(ptr);
}

void FUN_00401010(char16_t* title_buffer, const char* product_name, bool apply_to_window) noexcept {
    if (title_buffer == nullptr) {
        return;
    }

    std::size_t pos = 0;
    append_ascii(title_buffer, pos, product_name);
    append_wide_literal(title_buffer, pos, u" <Build:");
    append_ascii(title_buffer, pos, kBuildNumber);
    append_wide_literal(title_buffer, pos, u"> [ ");
    append_ascii(title_buffer, pos, kBuildConfig);
    append_wide_literal(title_buffer, pos, u" ] ");
    append_ascii(title_buffer, pos, kBuildTimestamp);
    append_wide_literal(title_buffer, pos, u" ");
    append_ascii(title_buffer, pos, kCheatsMarker);
    title_buffer[pos] = u'\0';

    if (apply_to_window) {
        re5::runtime::set_window_title_for_class(kWindowClass, title_buffer);
    }
}

void FUN_004011B0(
    void* first,
    std::uint32_t stride,
    std::uint32_t count,
    FUN_004011B0_Callback callback) noexcept {
    std::uint32_t counter = count - 1U;
    if ((counter & 0x80000000U) != 0U) {
        return;
    }

    auto cursor = reinterpret_cast<std::uintptr_t>(first);
    for (;;) {
        callback(reinterpret_cast<void*>(cursor));
        cursor += stride;
        counter -= 1U;
        if ((counter & 0x80000000U) != 0U) {
            break;
        }
    }
}

std::uintptr_t FUN_004011F0() noexcept {
    return 0x01652E00U;
}

void* FUN_00401200(std::size_t size) noexcept {
    return re5::runtime::aligned_allocate(size, 16U);
}

void FUN_00401210(std::uintptr_t /*unused*/, const char* text) noexcept {
    re5::runtime::debug_output_a(text);
}

bool FUN_00401220() noexcept {
    return false;
}

void FUN_00401230(std::uintptr_t /*unused*/) noexcept {}
void FUN_00401240(std::uintptr_t /*unused*/) noexcept {}

// Original VA 0x00401250: FF 25 FC 14 37 01
// Original VA 0x00401260: FF 25 FC 14 37 01
// Both are exact tail jumps through the same IAT slot: steam_api!SteamAPI_RunCallbacks.
void FUN_00401250() noexcept {
    re5::runtime::steam_run_callbacks();
}

void FUN_00401260() noexcept {
    re5::runtime::steam_run_callbacks();
}

void FUN_00401270_SetServices(const FUN_00401270_Services* services) noexcept {
    g_FUN_00401270_services = services;
}

void FUN_00401270() noexcept {
    const auto* services = g_FUN_00401270_services;
    if (services == nullptr || services->prepare_user_state == nullptr ||
        services->get_signin_info == nullptr || services->log_not_live == nullptr ||
        services->write_achievements == nullptr || services->mirror_achievement == nullptr) {
        return;
    }

    services->prepare_user_state(services->context);

    FUN_00401270_UserSigninInfo signin_info{};
    const std::uint32_t signin_result = services->get_signin_info(
        services->context, kPrimaryUserIndex, kSigninInfoFlags, &signin_info);
    if (signin_result == 0U && signin_info.signin_state != kSignedInToLive) {
        services->log_not_live(services->context, signin_info.user_name);
    }

    for (std::uint32_t achievement_id = 0; achievement_id < kAchievementCount;
         ++achievement_id) {
        const FUN_00401270_Achievement achievement{kPrimaryUserIndex, achievement_id};
        (void)services->write_achievements(
            services->context, 1U, &achievement, nullptr);
        services->mirror_achievement(services->context, achievement_id);
    }
}

void FUN_004012F0_SetServices(const FUN_004012F0_Services* services) noexcept {
    g_FUN_004012F0_services = services;
}

std::uint32_t FUN_004012F0(
    void** out_buffer,
    std::uint32_t requested_count,
    std::uint32_t /*unused*/) noexcept {
    const auto* services = g_FUN_004012F0_services;
    if (out_buffer == nullptr || services == nullptr || services->get_signin_info == nullptr ||
        services->create_achievement_enumerator == nullptr || services->enumerate == nullptr ||
        services->close_handle == nullptr) {
        return 0U;
    }

    if (requested_count == 0U) {
        requested_count = kDefaultEnumerationCount;
    }

    FUN_00401270_UserSigninInfo signin_info{};
    (void)services->get_signin_info(
        services->context, kPrimaryUserIndex, kSigninInfoFlags, &signin_info);

    std::uint32_t handle = kInvalidEnumerationHandle;
    std::uint32_t buffer_size = 0U;
    (void)services->create_achievement_enumerator(
        services->context,
        kResidentEvil5TitleId,
        kPrimaryUserIndex,
        0U,
        0xFFFFFFFFU,
        0U,
        requested_count,
        &buffer_size,
        &handle);

    void* buffer = re5::runtime::aligned_allocate(buffer_size, 16U);
    (void)services->enumerate(
        services->context, handle, buffer, buffer_size, &requested_count, nullptr);

    if (handle != 0U && handle != kInvalidEnumerationHandle) {
        services->close_handle(services->context, handle);
    }

    *out_buffer = buffer;
    return requested_count;
}

bool FUN_00401400(const FUN_00401400_Object* self) noexcept {
    return self->current == self->target;
}

void FUN_00401410_SetServices(const FUN_00401410_Services* services) noexcept {
    g_FUN_00401410_services = services;
}

void FUN_00401410() noexcept {
    const auto* services = g_FUN_00401410_services;
    if (services == nullptr || services->get_object == nullptr ||
        services->query_threshold == nullptr || services->dispatch == nullptr) {
        return;
    }

    void* query_object = services->get_object(services->context);
    if (!services->query_threshold(query_object, kFUN_00401410Key, kFUN_00401410Threshold)) {
        return;
    }

    void* dispatch_object = services->get_object(services->context);
    services->dispatch(dispatch_object);
}

void FUN_00401470_SetServices(const FUN_00401470_Services* services) noexcept {
    g_FUN_00401470_services = services;
}

std::uint32_t FUN_00401470() noexcept {
    const auto* services = g_FUN_00401470_services;
    if (services == nullptr || services->get_object == nullptr ||
        services->query_value == nullptr) {
        return 0U;
    }

    std::uint32_t value = kFUN_00401410Threshold;
    void* object = services->get_object(services->context);
    if (!services->query_value(object, kFUN_00401410Key, &value)) {
        return 0U;
    }
    if (value >= kFUN_00401410Threshold) {
        return 0U;
    }
    if (value == 0U) {
        return kFUN_00401470SuccessCode;
    }
    return 0U;
}

} // namespace re5::recovered
