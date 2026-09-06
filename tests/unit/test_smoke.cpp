#include <cstdint>

namespace re5 {
std::uint32_t recovered_runtime_abi_version() noexcept;
}

int main() {
    return re5::recovered_runtime_abi_version() == 1U ? 0 : 1;
}
