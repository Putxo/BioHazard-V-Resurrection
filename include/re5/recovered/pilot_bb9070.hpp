#pragma once
#include <cstddef>
#include <cstdint>

namespace re5::recovered {

// Provisional linked-node view used only for the BB9070/BB90C0 pilot cluster.
// The target is 32-bit, so field_04 is a four-byte pointer there. Packing keeps
// the observed +0x04 member offset in portable host-side validation builds.
// No class name or wider layout is inferred from this cluster.
#pragma pack(push, 4)
struct UnknownBB90Object {
    std::uint32_t field_00;
    const UnknownBB90Object* field_04;
};
#pragma pack(pop)

static_assert(offsetof(UnknownBB90Object, field_04) == 4);

[[nodiscard]] std::uint32_t FUN_00BB9070(const UnknownBB90Object* self) noexcept;
[[nodiscard]] const UnknownBB90Object* FUN_00BB9080(const UnknownBB90Object* self) noexcept;
[[nodiscard]] const UnknownBB90Object* FUN_00BB9090(const UnknownBB90Object* self) noexcept;

// Original ABI: ECX=self, stack arguments result and candidate, callee pops 8.
// Returns result after storing whether self occurs in candidate's field_04 chain.
[[nodiscard]] bool* FUN_00BB90C0(
    const UnknownBB90Object* self,
    bool* result,
    const UnknownBB90Object* candidate) noexcept;

} // namespace re5::recovered
