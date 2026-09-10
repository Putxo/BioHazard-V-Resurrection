#include "re5/recovered/fun_00401fe0.hpp"

#include <cstdarg>
#include <cstdio>

namespace re5::recovered {

int FUN_00401FE0(char* destination, const char* format, ...) noexcept {
    va_list args;
    va_start(args, format);
    const int result = std::vsnprintf(destination, 0x800U, format, args);
    va_end(args);
    return result;
}

} // namespace re5::recovered
