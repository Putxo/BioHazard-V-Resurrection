#include "re5/recovered/batch_00408b50.hpp"
#include <array>
#include <cstdlib>
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>
using namespace re5::recovered;
using U32 = Batch408_U32;
namespace {
constexpr U32 self = 0x02000000U;
constexpr U32 registry = 0x0165A1F0U;
unsigned checks = 0;
void check(bool condition, const char* text) {
    ++checks;
    if (!condition) throw std::runtime_error(text);
}
struct Call { U32 target, object; std::vector<U32> args; };
struct Fixture {
    std::map<U32, U32> memory;
    std::vector<Call> calls;
    U32 mode = 0, status = 0, mask = 15, loaded = 0;
    bool mutate = false;
    Fixture() {
        for (U32 off = 0; off < 0x40; off += 4) memory[self + off] = 0x55550000U + off;
        memory[self + 0x34] = 0xFFFFFFFFU;
        memory[registry] = 0x31000000;
        memory[0x0165BBB0] = 0x32000000;
        memory[0x016E1D3C] = 0x33000000;
        memory[0x016E2EFC] = 0x34000000;
        memory[0x016E1660] = 0x35000000;
        memory[0x3501E4EC] = 0x36000000;
        memory[0x36000058] = 0;
        memory[0x0137119C] = 0x70000000;
        for (U32 i = 0; i < 8; ++i) memory[0x01568444 + i * 4] = 0x40000000 + i;
    }
    static U32 read(void* raw, U32 address) {
        auto& f = *static_cast<Fixture*>(raw);
        auto it = f.memory.find(address);
        if (it == f.memory.end()) throw std::runtime_error("unexpected target-memory read");
        return it->second;
    }
    static void write(void* raw, U32 address, U32 value) {
        auto& f = *static_cast<Fixture*>(raw);
        check(f.memory.count(address) != 0, "unexpected target-memory write");
        f.memory[address] = value;
    }
    static U32 invoke(void* raw, U32 target, U32 object, const U32* args, U32 count) {
        auto& f = *static_cast<Fixture*>(raw);
        Call call{target, object, {}};
        for (U32 i = 0; i < count; ++i) call.args.push_back(args[i]);
        f.calls.push_back(call);
        if (target == 0x70000000) {
            check(object == 0 && count == 1 && args[0] == self + 8, "stdcall import arguments");
            check(f.memory.at(self) == 0x0137A990U, "intermediate destructor vtable");
        }
        if (f.mutate) {
            f.memory[registry] += 0x10;
            f.memory[0x016E1D3C] += 0x10;
            if (target == 0x004B1430) f.memory[self + 0x34] = 0x80000001U;
            if (target == 0x004B14A0) f.memory[self + 0x34] = 123U;
            if (target == 0x00D02A20) {
                for (U32 off = 0; off < 0x40; off += 4) f.memory[self + off] = 0x66660000U + off;
            }
        }
        if (target == 0x004E6F10) return f.mode;
        if (target == 0x004B1470) return f.status;
        if (target == 0x00D14C10) {
            const U32 index = f.loaded++;
            check(index < 4 && count == 3 && args[2] == 1, "resource load arguments/count");
            return ((f.mask >> index) & 1U) ? 0x50000000U + index * 0x100U : 0;
        }
        return 0xABCDEF01U;
    }
    Batch408_Access access() { return {this, read, write, invoke}; }
};

void guards_and_status() {
    const std::array<U32, 6> handles{0, 1, 0x7FFFFFFF, 0x80000000, 0x80000001, 0xFFFFFFFF};
    const std::array<U32, 7> states{0, 1, 2, 3, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF};
    for (U32 h : handles) for (U32 s : states) {
        Fixture f; auto a = f.access();
        f.memory[self + 0x34] = h; f.status = s;
        if (h & 0x80000000U) f.memory.erase(registry); // unreadable after the guard
        const bool result = FUN_00408B90(a, self);
        const bool active = (h & 0x80000000U) == 0;
        check(result == (!active || s == 0 || s == 2), "status result");
        check(f.calls.size() == (active ? 1U : 0U), "status early exit");
        check(f.memory.at(self + 0x34) == (active && s == 0 ? 0xFFFFFFFFU : h), "status clear semantics");
        f.calls.clear(); f.memory[self + 0x34] = h;
        FUN_00408BC0(a, self);
        check(f.calls.size() == (active ? 1U : 0U), "stop early exit");
        f.calls.clear(); f.memory[self + 0x34] = h; f.mutate = true;
        FUN_00408BE0(a, self);
        check(f.calls.size() == (active ? 2U : 0U), "stop/cancel early exit");
        if (active) {
            check(f.calls[1].args[0] == 0x80000001U, "reload changed negative handle without another guard");
            check(f.calls[1].object == f.calls[0].object + 0x10U, "reload changed registry");
            check(f.memory.at(self + 0x34) == 0xFFFFFFFFU, "clear after cancellation");
        }
    }
    for (U32 h : handles) for (U32 mode : states) {
        Fixture f; auto a = f.access(); f.memory[self + 0x34] = h; f.mode = mode; f.mutate = true;
        check(FUN_00408B50(a, self) == 0xABCDEF01U, "registration return propagation");
        const U32 selector = mode ? 0x9DU : 0x9EU;
        check(f.memory.at(self + 0x34) == selector, "registration selector/store");
        check(f.calls.back().args == std::vector<U32>({selector, self}), "registration order");
        check(f.calls.size() == ((h & 0x80000000U) ? 2U : 3U), "conditional prior cancellation");
    }
}
void selectors() {
    using Fn = U32 (*)(Batch408_Access&, U32);
    const std::array<Fn, 3> fn{FUN_00408C10, FUN_00408C30, FUN_00408C50};
    const std::array<U32, 3> code{0x98, 0x99, 0xA2};
    for (unsigned i = 0; i < 3; ++i) {
        Fixture f; auto a = f.access();
        check(fn[i](a, 0xDEADBEEFU) == 0xABCDEF01U, "constant selector return");
        check(f.calls[0].args == std::vector<U32>({code[i], 0xDEADBEEFU}), "opaque self forwarded");
    }
    for (U32 mode : {0U, 1U, 2U, 3U, 4U, 0x80000000U, 0xFFFFFFFFU}) {
        Fixture f; auto a = f.access(); f.memory[0x36000058] = mode;
        FUN_00408C70(a, self); FUN_00408CA0(a, self);
        check(f.calls[0].args[0] == (mode == 2 ? 0xA0U : 0xD1U), "selector mode=2");
        check(f.calls[1].args[0] == (mode == 3 ? 0xCDU : 0xCFU), "selector mode=3");
    }
    for (U32 high : {0U, 0x100U, 0x7FFFFF00U, 0xFFFFFF00U}) for (U32 low = 0; low < 256; ++low) {
        Fixture f; auto a = f.access();
        FUN_00408CD0(a, self, high | low);
        check(f.calls[0].args[0] == (low == 0 ? 1U : 0U), "low-byte bool inversion");
    }
}
void resources() {
    const std::array<U32, 4> types{0x016E2DB8, 0x01693688, 0x016E2830, 0x016E22B0};
    const std::array<U32, 4> delivery{0x004AFD20, 0x004AFD40, 0x004B13F0, 0x004C5060};
    for (U32 variant : {0U, 1U, 0x100U, 0x80000000U, 0xFFFFFFFFU}) for (U32 mask = 0; mask < 16; ++mask) {
        Fixture f; auto a = f.access(); f.mask = mask; f.mutate = true;
        FUN_00408CF0(a, variant);
        unsigned at = 0;
        for (U32 i = 0; i < 4; ++i) {
            const auto& load = f.calls.at(at++);
            check(load.target == 0x00D14C10 && load.args[0] == types[i], "resource type/order");
            check(load.args[1] == 0x40000000U + i + (variant ? 4U : 0U), "full-word variant selection");
            if ((mask >> i) & 1U) {
                const auto& deliver = f.calls.at(at++);
                const auto& release = f.calls.at(at++);
                check(deliver.target == delivery[i] && deliver.args[0] == 0x50000000U + i * 0x100U, "deliver exact returned resource");
                check(release.target == 0x00D25060 && release.object == deliver.args[0] && release.args.empty(), "balanced release");
            }
        }
        check(at == f.calls.size(), "resource null guards");
    }
    Fixture f; auto a = f.access(); f.mask = 0;
    FUN_00408DD0(a);
    check(f.calls.size() == 4 && f.calls[0].args[1] == 0x40000000U, "zero-variant wrapper");
}
void construction() {
    Fixture f; auto a = f.access(); f.mutate = true;
    check(FUN_00408DE0(a, self) == self, "constructor return");
    check(f.calls.size() == 1 && f.calls[0].target == 0x00D02A20 && f.calls[0].object == self, "base constructor first");
    for (U32 off = 0; off < 0x40; off += 4) {
        const U32 expected = off == 0 ? 0x0137E4A8U : (off == 0x28 || off == 0x30) ? 0U : 0x66660000U + off;
        check(f.memory.at(self + off) == expected, "constructor preserves all other base fields");
    }
    f.calls.clear(); FUN_00408E00(a, self);
    check(f.calls.size() == 1 && f.memory.at(self) == 0x0137A97CU, "destructor final vtable");
}
} // namespace
int main() {
    try {
        guards_and_status(); selectors(); resources(); construction();
        std::cout << "batch_00408b50: PASS " << checks << " explicit checks; all 14 entrypoints\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "batch_00408b50: FAIL: " << e.what() << '\n';
        return 1;
    }
}
