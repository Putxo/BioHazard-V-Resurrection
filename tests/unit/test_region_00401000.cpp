#include "re5/recovered/region_00401000.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <string_view>
#include <vector>

namespace {
std::array<std::uintptr_t, 4> g_seen{};
std::size_t g_seen_count = 0;

void record_callback(void* ptr) noexcept {
    if (g_seen_count < g_seen.size()) {
        g_seen[g_seen_count++] = reinterpret_cast<std::uintptr_t>(ptr);
    }
}

struct AchievementProbe {
    std::uint32_t prepare_count{};
    std::uint32_t signin_result{};
    std::uint32_t signin_state{};
    std::uint32_t signin_user_index{};
    std::uint32_t signin_flags{};
    std::string logged_user{};
    std::vector<re5::recovered::FUN_00401270_Achievement> writes;
    std::vector<std::uint32_t> mirrored_ids;
};

void prepare_user_state(void* context) noexcept {
    ++static_cast<AchievementProbe*>(context)->prepare_count;
}

std::uint32_t get_signin_info(
    void* context,
    std::uint32_t user_index,
    std::uint32_t flags,
    re5::recovered::FUN_00401270_UserSigninInfo* info) noexcept {
    auto& probe = *static_cast<AchievementProbe*>(context);
    probe.signin_user_index = user_index;
    probe.signin_flags = flags;
    info->signin_state = probe.signin_state;
    std::memcpy(info->user_name, "TestUser", 9U);
    return probe.signin_result;
}

void log_not_live(void* context, const char* user_name) noexcept {
    static_cast<AchievementProbe*>(context)->logged_user = user_name;
}

std::uint32_t write_achievements(
    void* context,
    std::uint32_t count,
    const re5::recovered::FUN_00401270_Achievement* achievements,
    void* overlapped) noexcept {
    assert(count == 1U);
    assert(overlapped == nullptr);
    static_cast<AchievementProbe*>(context)->writes.push_back(*achievements);
    return 0U;
}

void mirror_achievement(void* context, std::uint32_t achievement_id) noexcept {
    static_cast<AchievementProbe*>(context)->mirrored_ids.push_back(achievement_id);
}

struct AchievementEnumerationProbe {
    std::uint32_t signin_user_index{};
    std::uint32_t signin_flags{};
    std::uint32_t title_id{};
    std::uint32_t enum_user_index{};
    std::uint64_t xuid{};
    std::uint32_t detail_flags{};
    std::uint32_t starting_index{};
    std::uint32_t requested_count{};
    std::uint32_t supplied_buffer_size{0x40U};
    std::uint32_t supplied_handle{0x1234U};
    std::uint32_t enumerated_handle{};
    void* enumerated_buffer{};
    std::uint32_t enumerated_buffer_size{};
    std::uint32_t returned_count{12U};
    std::uint32_t close_count{};
    std::uint32_t closed_handle{};
};

std::uint32_t enumeration_get_signin_info(
    void* context,
    std::uint32_t user_index,
    std::uint32_t flags,
    re5::recovered::FUN_00401270_UserSigninInfo* /*info*/) noexcept {
    auto& probe = *static_cast<AchievementEnumerationProbe*>(context);
    probe.signin_user_index = user_index;
    probe.signin_flags = flags;
    return 0U;
}

std::uint32_t create_achievement_enumerator(
    void* context,
    std::uint32_t title_id,
    std::uint32_t user_index,
    std::uint64_t xuid,
    std::uint32_t detail_flags,
    std::uint32_t starting_index,
    std::uint32_t item_count,
    std::uint32_t* buffer_size,
    std::uint32_t* handle) noexcept {
    auto& probe = *static_cast<AchievementEnumerationProbe*>(context);
    probe.title_id = title_id;
    probe.enum_user_index = user_index;
    probe.xuid = xuid;
    probe.detail_flags = detail_flags;
    probe.starting_index = starting_index;
    probe.requested_count = item_count;
    *buffer_size = probe.supplied_buffer_size;
    *handle = probe.supplied_handle;
    return 0U;
}

std::uint32_t enumerate_achievements(
    void* context,
    std::uint32_t handle,
    void* buffer,
    std::uint32_t buffer_size,
    std::uint32_t* items_returned,
    void* overlapped) noexcept {
    auto& probe = *static_cast<AchievementEnumerationProbe*>(context);
    assert(overlapped == nullptr);
    probe.enumerated_handle = handle;
    probe.enumerated_buffer = buffer;
    probe.enumerated_buffer_size = buffer_size;
    *items_returned = probe.returned_count;
    return 0U;
}

void close_enumeration_handle(void* context, std::uint32_t handle) noexcept {
    auto& probe = *static_cast<AchievementEnumerationProbe*>(context);
    ++probe.close_count;
    probe.closed_handle = handle;
}
} // namespace

void test_region_00401000() {
    using namespace re5::recovered;

    std::array<char16_t, 0x104> title{};
    FUN_00401010(title.data(), "Resident Evil 5", false);
    assert(std::u16string_view(title.data()) ==
           u"Resident Evil 5 <Build:3170> [ RELEASE ] Thu Apr 16, 2015 06:51:32 PM (CHEATS)");

    std::array<char16_t, 0x104> null_product{};
    FUN_00401010(null_product.data(), nullptr, false);
    assert(std::u16string_view(null_product.data()) ==
           u" <Build:3170> [ RELEASE ] Thu Apr 16, 2015 06:51:32 PM (CHEATS)");

    g_seen = {};
    g_seen_count = 0;
    alignas(16) std::array<std::byte, 64> storage{};
    FUN_004011B0(storage.data(), 8U, 3U, &record_callback);
    assert(g_seen_count == 3U);

    const auto base = reinterpret_cast<std::uintptr_t>(storage.data());
    assert(g_seen[0] == base);
    assert(g_seen[1] == base + 8U);
    assert(g_seen[2] == base + 16U);

    g_seen_count = 0;
    FUN_004011B0(storage.data(), 8U, 0U, &record_callback);
    assert(g_seen_count == 0U);

    assert(FUN_004011F0() == 0x01652E00U);
    assert(!FUN_00401220());

    FUN_00401230(0x1234U);
    FUN_00401240(0x5678U);

    void* aligned = FUN_00401200(64U);
    assert(aligned != nullptr);
    assert((reinterpret_cast<std::uintptr_t>(aligned) & 0xFU) == 0U);
    FUN_00401000(aligned);

    // On the original Win32 target both are direct import thunks to the same
    // SteamAPI_RunCallbacks IAT slot. On non-Windows CI the import adapter is inert.
    FUN_00401250();
    FUN_00401260();

    AchievementProbe achievements{};
    achievements.signin_state = 1U;
    const FUN_00401270_Services services{
        &achievements,
        &prepare_user_state,
        &get_signin_info,
        &log_not_live,
        &write_achievements,
        &mirror_achievement,
    };
    FUN_00401270_SetServices(&services);
    FUN_00401270();
    FUN_00401270_SetServices(nullptr);

    assert(achievements.prepare_count == 1U);
    assert(achievements.signin_user_index == 0U);
    assert(achievements.signin_flags == 1U);
    assert(achievements.logged_user == "TestUser");
    assert(achievements.writes.size() == 0x46U);
    assert(achievements.mirrored_ids.size() == 0x46U);
    for (std::uint32_t id = 0; id < 0x46U; ++id) {
        assert(achievements.writes[id].user_index == 0U);
        assert(achievements.writes[id].achievement_id == id);
        assert(achievements.mirrored_ids[id] == id);
    }

    AchievementProbe live_user{};
    live_user.signin_state = 2U;
    const FUN_00401270_Services live_services{
        &live_user,
        &prepare_user_state,
        &get_signin_info,
        &log_not_live,
        &write_achievements,
        &mirror_achievement,
    };
    FUN_00401270_SetServices(&live_services);
    FUN_00401270();
    FUN_00401270_SetServices(nullptr);
    assert(live_user.logged_user.empty());

    AchievementEnumerationProbe enumeration{};
    const FUN_004012F0_Services enumeration_services{
        &enumeration,
        &enumeration_get_signin_info,
        &create_achievement_enumerator,
        &enumerate_achievements,
        &close_enumeration_handle,
    };

    void* achievement_details = nullptr;
    FUN_004012F0_SetServices(&enumeration_services);
    const auto enumerated_count = FUN_004012F0(&achievement_details, 0x46U, 0xDEADBEEFU);
    FUN_004012F0_SetServices(nullptr);

    assert(enumerated_count == 12U);
    assert(enumeration.signin_user_index == 0U);
    assert(enumeration.signin_flags == 1U);
    assert(enumeration.title_id == 0x434307F7U);
    assert(enumeration.enum_user_index == 0U);
    assert(enumeration.xuid == 0U);
    assert(enumeration.detail_flags == 0xFFFFFFFFU);
    assert(enumeration.starting_index == 0U);
    assert(enumeration.requested_count == 0x46U);
    assert(enumeration.enumerated_handle == 0x1234U);
    assert(enumeration.enumerated_buffer == achievement_details);
    assert(enumeration.enumerated_buffer_size == 0x40U);
    assert(achievement_details != nullptr);
    assert((reinterpret_cast<std::uintptr_t>(achievement_details) & 0xFU) == 0U);
    assert(enumeration.close_count == 1U);
    assert(enumeration.closed_handle == 0x1234U);
    FUN_00401000(achievement_details);

    AchievementEnumerationProbe default_count{};
    default_count.supplied_handle = 0U;
    default_count.returned_count = 7U;
    void* default_details = nullptr;
    const FUN_004012F0_Services default_count_services{
        &default_count,
        &enumeration_get_signin_info,
        &create_achievement_enumerator,
        &enumerate_achievements,
        &close_enumeration_handle,
    };
    FUN_004012F0_SetServices(&default_count_services);
    const auto default_result = FUN_004012F0(&default_details, 0U, 0U);
    FUN_004012F0_SetServices(nullptr);

    assert(default_result == 7U);
    assert(default_count.requested_count == 0xFFU);
    assert(default_count.enumerated_handle == 0U);
    assert(default_count.close_count == 0U);
    FUN_00401000(default_details);

    const FUN_00401400_Object matched{0xABCDEF01U, 0x42U, 0x42U};
    assert(FUN_00401400(&matched));

    const FUN_00401400_Object mismatched{0xABCDEF01U, 0x42U, 0x43U};
    assert(!FUN_00401400(&mismatched));
}
