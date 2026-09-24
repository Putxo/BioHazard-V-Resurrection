#include "re5/recovered/fun_00407b70.hpp"

namespace re5::recovered {
namespace {
constexpr std::uint32_t kFallbackString = 0x0137AA5CU;
} // namespace

std::uint32_t FUN_00407B70(const FUN_00407B70_Object& object) noexcept {
    const std::uint32_t string_object = object.string_object;
    if (string_object == 0U) {
        return kFallbackString;
    }

    return string_object + 8U;
}

} // namespace re5::recovered
