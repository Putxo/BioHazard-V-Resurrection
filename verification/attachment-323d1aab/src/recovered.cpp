#include "recovered.hpp"

namespace re5::recovered {
namespace {
// Original fallback at VA 0x0137AA5C starts with 00. The immutable empty
// string is relocated here, rather than retaining an EXE-only address.
const byte empty_string[1] = {0};

u32 load_le32(const byte* p) noexcept {
    return static_cast<u32>(p[0]) |
           (static_cast<u32>(p[1]) << 8u) |
           (static_cast<u32>(p[2]) << 16u) |
           (static_cast<u32>(p[3]) << 24u);
}
} // namespace

FourByteObject* RE5_MEMBERCALL FourByteObject::FUN_004022A0(
    u32 arg1, u32 arg2, u32 arg3, u32 arg4) noexcept {
    // 004022AA / 004022B0 / 004022B7 / 004022BA, in original store order.
    fields[0] = static_cast<byte>(arg3);
    fields[1] = static_cast<byte>(arg2);
    fields[2] = static_cast<byte>(arg1);
    fields[3] = static_cast<byte>(arg4);
    return this;
}

s32 RE5_MEMBERCALL RawStringObject::FUN_004022F0(const byte* argument) const noexcept {
    if (storage == nullptr) {
        // 00402321..0040233E: exactly one byte compared against the empty
        // fallback. No read of a second byte, and no null-argument guard.
        return argument[0] == 0 ? 0 : 1;
    }

    const byte* stored = storage + 8;
    for (;;) {
        // Preserve the original two-byte loop and both early NUL exits.
        const byte first = argument[0];
        if (first != stored[0]) {
            return first < stored[0] ? -1 : 1;
        }
        if (first == 0) {
            return 0;
        }
        const byte second = argument[1];
        if (second != stored[1]) {
            return second < stored[1] ? -1 : 1;
        }
        argument += 2;
        stored += 2;
        if (second == 0) {
            return 0;
        }
    }
}

u32 RE5_MEMBERCALL RawStringObject::FUN_00402350() const noexcept {
    // 00402354 skips the storage read when [this] is null.
    return storage == nullptr ? 0u : load_le32(storage + 4);
}

const byte* RE5_MEMBERCALL RawStringObject::FUN_00402420() const noexcept {
    // No allocation or copying. The nonempty result aliases the original block.
    return storage == nullptr ? empty_string : storage + 8;
}
} // namespace re5::recovered
