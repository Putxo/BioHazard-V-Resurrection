#include "re5/recovered/fun_00406910.hpp"

#include <cassert>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

namespace {
using re5::recovered::FUN_00406910_Services;

struct State {
    std::vector<std::string> events;
    std::uint8_t mode = 7U;
    bool special = false;
    bool ready = false;
    bool suppressed_a = false;
    bool suppressed_b = false;
    const char* root = "native";
    void* archive_handle = reinterpret_cast<void*>(0x1111U);
    void* sound_handle = reinterpret_cast<void*>(0x2222U);
};

void event(State& s, const std::string& value) {
    s.events.push_back(value);
}

void trace_marker(void* context, const char* marker) noexcept {
    auto& s = *static_cast<State*>(context);
    event(s, std::string{"trace:"} + (marker != nullptr ? marker : ""));
}

std::uint8_t get_convert_mode(void* context) noexcept {
    return static_cast<State*>(context)->mode;
}

void set_convert_mode(void* context, std::uint8_t value) noexcept {
    auto& s = *static_cast<State*>(context);
    s.mode = value;
    event(s, "mode:" + std::to_string(value));
}

const char* resource_root(void* context) noexcept {
    return static_cast<State*>(context)->root;
}

bool special_mode_enabled(void* context) noexcept {
    return static_cast<State*>(context)->special;
}

void prepare_archive(void* context, const char* path) noexcept {
    event(*static_cast<State*>(context), std::string{"prepare:"} + path);
}

void* resolve_resource(
    void* context,
    std::uintptr_t table,
    const char* name,
    bool create_if_missing) noexcept {
    auto& s = *static_cast<State*>(context);
    event(
        s,
        "resolve:" + std::to_string(table) + ":" + name +
            ":" + (create_if_missing ? "1" : "0"));
    if (table == 0x016E216CU) {
        return s.archive_handle;
    }
    if (table == 0x016E2830U) {
        return s.sound_handle;
    }
    return nullptr;
}

void activate_resource(void* context, void*) noexcept {
    event(*static_cast<State*>(context), "activate");
}

bool archive_ready(void* context, const char* path) noexcept {
    auto& s = *static_cast<State*>(context);
    event(s, std::string{"ready:"} + path);
    return s.ready;
}

void convert_archive(void* context, const char* name) noexcept {
    event(*static_cast<State*>(context), std::string{"convert:"} + name);
}

void flush_archive_conversion(void* context) noexcept {
    event(*static_cast<State*>(context), "flush");
}

bool registry_suppressed_a(void* context) noexcept {
    return static_cast<State*>(context)->suppressed_a;
}

bool registry_suppressed_b(void* context) noexcept {
    return static_cast<State*>(context)->suppressed_b;
}

void register_resolved(void* context, void*) noexcept {
    event(*static_cast<State*>(context), "register");
}

void convert_directory(
    void* context,
    const char* base_path,
    const char* type_key,
    const char* extension) noexcept {
    event(
        *static_cast<State*>(context),
        std::string{"dir:"} + base_path + ":" + type_key + ":" + extension);
}

void finish_conversion(void* context, std::int32_t value) noexcept {
    event(*static_cast<State*>(context), "finish:" + std::to_string(value));
}

FUN_00406910_Services make_services(State& state) {
    return FUN_00406910_Services{
        &state,
        trace_marker,
        get_convert_mode,
        set_convert_mode,
        resource_root,
        special_mode_enabled,
        prepare_archive,
        resolve_resource,
        activate_resource,
        archive_ready,
        convert_archive,
        flush_archive_conversion,
        registry_suppressed_a,
        registry_suppressed_b,
        register_resolved,
        convert_directory,
        finish_conversion,
    };
}
} // namespace

void test_fun_00406910() {
    using namespace re5::recovered;

    // Native early exit: the unsigned range check rejects everything outside
    // 0x190..0x197 before the conversion body touches global state.
    State range_state{};
    auto range_services = make_services(range_state);
    FUN_00406910_SetServices(&range_services);
    FUN_00406910(0x18F);
    FUN_00406910(0x198);
    assert(range_state.events.empty());
    assert(range_state.mode == 7U);

    // Portable-host guard only: no service block.
    FUN_00406910_SetServices(nullptr);
    FUN_00406910(0x190);

    // Portable-host guard only: incomplete callback table.
    State incomplete_state{};
    auto incomplete_services = make_services(incomplete_state);
    incomplete_services.convert_archive = nullptr;
    FUN_00406910_SetServices(&incomplete_services);
    FUN_00406910(0x190);
    assert(incomplete_state.events.empty());
    assert(incomplete_state.mode == 7U);

    // Normal path, exact stage 0x190 formatting and call order.
    State normal{};
    auto normal_services = make_services(normal);
    FUN_00406910_SetServices(&normal_services);
    FUN_00406910(0x190);

    assert(normal.mode == 7U);
    assert(normal.events.size() == 12U);
    assert(normal.events[0].find("trace:>>>>>>>>") == 0U);
    assert(normal.events[1] == "mode:1");
    assert(normal.events[2] == "convert:Image\\Archive\\s400Sub");
    assert(normal.events[3] == "flush");
    assert(normal.events[4] == "resolve:23996464:sound\\bgm_dc\\s4xx_radio:1");
    assert(normal.events[5] == "register");
    assert(normal.events[6] == "dir:stage\\s400\\soft\\mercedc:rLayout:.lot");
    assert(normal.events[7] == "dir:stage\\s400\\soft\\mercedc:rScenario:.sce");
    assert(normal.events[8] == "dir:stage\\s400\\soft\\mercedc:rAIFSM:.fsm");
    assert(normal.events[9] == "finish:0");
    assert(normal.events[10] == "mode:7" ||
           normal.events[10].find("trace:<<<<<<<<") == 0U);

    // The final two events are restore then closing marker.
    assert(normal.events.size() >= 2U);
    assert(normal.events[normal.events.size() - 2U] == "mode:7");
    assert(normal.events.back().find("trace:<<<<<<<<") == 0U);

    // Special-mode early completion corresponds to the native JNE 0x406AA7:
    // prepare + resolve/activate + ready, then restore/trace, with no normal
    // convert/flush/sound/directory/finalize work.
    State special{};
    special.special = true;
    special.ready = true;
    special.root = "root";
    auto special_services = make_services(special);
    FUN_00406910_SetServices(&special_services);
    FUN_00406910(0x197);

    assert(special.mode == 7U);
    bool saw_prepare = false;
    bool saw_activate = false;
    bool saw_convert = false;
    bool saw_finish = false;
    for (const auto& e : special.events) {
        if (e == "prepare:root\\Image\\Archive\\s407Sub.arc") saw_prepare = true;
        if (e == "activate") saw_activate = true;
        if (e.find("convert:") == 0U) saw_convert = true;
        if (e.find("finish:") == 0U) saw_finish = true;
    }
    assert(saw_prepare);
    assert(saw_activate);
    assert(!saw_convert);
    assert(!saw_finish);
    assert(special.events[special.events.size() - 2U] == "mode:7");
    assert(special.events.back().find("trace:<<<<<<<<") == 0U);

    // Registry suppression prevents the native registration call but does not
    // suppress the remainder of normal conversion.
    State suppressed{};
    suppressed.suppressed_a = true;
    auto suppressed_services = make_services(suppressed);
    FUN_00406910_SetServices(&suppressed_services);
    FUN_00406910(0x191);
    for (const auto& e : suppressed.events) {
        assert(e != "register");
    }

    FUN_00406910_SetServices(nullptr);
}
