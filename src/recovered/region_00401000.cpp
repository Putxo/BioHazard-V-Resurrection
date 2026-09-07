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

void append_ascii(char16_t* out, std::size_t& pos, const char* text) noexcept {
    if (out == nullptr || text == nullptr) return;
    while (*text != '\0' && pos + 1U < kBuildTitleCapacity) {
        out[pos++] = static_cast<unsigned char>(*text++);
    }
}

void append_wide_literal(char16_t* out, std::size_t& pos, const char16_t* text) noexcept {
    if (out == nullptr || text == nullptr) return;
    while (*text != u'\0' && pos + 1U < kBuildTitleCapacity) {
        out[pos++] = *text++;
    }
}
} // namespace

// Original VA: 0x00401000
// RVA:         0x00001000
// x86: jmp 0x0125145C
// 0x0125145C is the target CRT aligned-free implementation.
void FUN_00401000(void* ptr) noexcept {
    re5::runtime::aligned_release(ptr);
}

// Original VA: 0x00401010
// RVA:         0x00001010
// The original builds four narrow local strings from .rdata and passes them,
// together with product_name, to the target wide printf wrapper using:
//   L"%S <Build:%S> [ %S ] %S %S"
// Exact embedded values are preserved below. The third argument is tested as a
// byte; when non-zero the target performs FindWindowW(L"MTFramework", nullptr)
// followed by SetWindowTextW(hwnd, title_buffer).
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

// Original VA: 0x004011B0
// RVA:         0x000011B0
// Original ABI: callee pops four stack arguments (ret 0x10). The callback is
// invoked with the current element in ECX, then the pointer advances by stride.
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

// Original VA: 0x004011F0
// RVA:         0x000011F0
// x86: mov eax,0x01652E00 ; ret
std::uintptr_t FUN_004011F0() noexcept {
    return 0x01652E00U;
}

// Original VA: 0x00401200
// RVA:         0x00001200
void* FUN_00401200(std::size_t size) noexcept {
    return re5::runtime::aligned_allocate(size, 16U);
}

// Original VA: 0x00401210
// RVA:         0x00001210
void FUN_00401210(std::uintptr_t /*unused*/, const char* text) noexcept {
    re5::runtime::debug_output_a(text);
}

// Original VA: 0x00401220
// RVA:         0x00001220
bool FUN_00401220() noexcept {
    return false;
}

// Original VAs: 0x00401230 / 0x00401240
void FUN_00401230(std::uintptr_t /*unused*/) noexcept {}
void FUN_00401240(std::uintptr_t /*unused*/) noexcept {}

} // namespace re5::recovered
