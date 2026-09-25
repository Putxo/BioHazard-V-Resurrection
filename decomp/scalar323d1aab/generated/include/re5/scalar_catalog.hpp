#pragma once
#include "re5/scalars.hpp"
namespace re5::scalar323d1aab {
enum class Kind { noop, zero8, constant8, store_arg32, load8, store_imm8, bitfield32, store_imm32, store_arg8, bitfield8, store_arg16, global_copy32, global_store32 };
using Action=void(*)() noexcept;
using G8=u8(RE5_SCALAR_CC*)(const void*) noexcept;
using G32=u32(RE5_SCALAR_CC*)(const void*) noexcept;
using W8=void(RE5_SCALAR_CC*)(void*,u8) noexcept;
using W16=void(RE5_SCALAR_CC*)(void*,u16) noexcept;
using W32=void(RE5_SCALAR_CC*)(void*,u32) noexcept;
using Store=void(RE5_SCALAR_CC*)(void*) noexcept;
using Copy=u32(*)() noexcept;
struct Entry {
    u32 va;Kind kind;u32 offset,imm,shift,src,dst,stack_pop;
    Action action;G8 g8;G32 g32;W8 w8;W16 w16;W32 w32;Store store;Copy copy;
};
extern const Entry entries[];
extern const unsigned entry_count;
}
