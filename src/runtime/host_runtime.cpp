#include "re5/runtime/host_runtime.hpp"

#include <cstdlib>

#if defined(_MSC_VER)
#include <malloc.h>
#endif

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <cstdio>
#endif

namespace re5::runtime {

void* aligned_allocate(std::size_t size, std::size_t alignment) noexcept {
#if defined(_MSC_VER)
    return _aligned_malloc(size, alignment);
#else
    if (alignment < sizeof(void*) || (alignment & (alignment - 1U)) != 0U) {
        return nullptr;
    }

    void* result = nullptr;
    if (posix_memalign(&result, alignment, size) != 0) {
        return nullptr;
    }
    return result;
#endif
}

void aligned_release(void* ptr) noexcept {
#if defined(_MSC_VER)
    _aligned_free(ptr);
#else
    std::free(ptr);
#endif
}

void debug_output_a(const char* text) noexcept {
    if (text == nullptr) {
        return;
    }
#if defined(_WIN32)
    ::OutputDebugStringA(text);
#else
    // CI/non-Windows fallback for the target's debugger-output side effect.
    std::fputs(text, stderr);
#endif
}

} // namespace re5::runtime
