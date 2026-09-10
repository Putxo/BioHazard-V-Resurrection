#include "re5/recovered/fun_004015d0.hpp"

#include <array>
#include <cstring>
#include <utility>

namespace re5::recovered {
namespace {
constexpr int kUnknownCountry = 48;

constexpr std::array<std::pair<const char*, int>, 49> kCountryMap{{
    {"AU", 1}, {"AT", 2}, {"BH", 3}, {"BE", 4}, {"BG", 5},
    {"CA", 6}, {"HR", 7}, {"CZ", 8}, {"DK", 9}, {"FI", 10},
    {"FR", 11}, {"DE", 12}, {"GB", 13}, {"GR", 14}, {"HK", 15},
    {"HU", 16}, {"IS", 17}, {"IN", 18}, {"IE", 19}, {"IL", 20},
    {"IT", 21}, {"JP", 22}, {"KR", 23}, {"KW", 24}, {"LB", 25},
    {"LU", 26}, {"NL", 27}, {"NZ", 28}, {"NO", 29}, {"OM", 30},
    {"PL", 31}, {"PT", 32}, {"QA", 33}, {"RO", 34}, {"RU", 35},
    {"SA", 36}, {"SG", 37}, {"SK", 38}, {"SI", 39}, {"ZA", 40},
    {"ES", 41}, {"SE", 42}, {"CH", 43}, {"TW", 44}, {"TR", 45},
    {"AE", 46}, {"US", 47}, {"UK", 13}, {"XX", 48},
}};

const FUN_004015D0_Services* g_FUN_004015D0_services = nullptr;
} // namespace

void FUN_004015D0_SetServices(const FUN_004015D0_Services* services) noexcept {
    g_FUN_004015D0_services = services;
}

int FUN_004015D0() noexcept {
    const auto* services = g_FUN_004015D0_services;
    if (services == nullptr || services->get_ip_country == nullptr) {
        return kUnknownCountry;
    }

    const char* country = services->get_ip_country(services->context);
    if (country == nullptr) {
        return kUnknownCountry;
    }

    for (const auto& [code, value] : kCountryMap) {
        if (std::strcmp(country, code) == 0) {
            return value;
        }
        if (std::strcmp(code, "XX") == 0) {
            break;
        }
    }
    return kUnknownCountry;
}

} // namespace re5::recovered
