#pragma once

namespace re5::recovered {

struct FUN_004018F0_Services {
    void* context;
    void* global_object;
    void (*dispatch_false_path)(void* context, void* object) noexcept;
    void (*dispatch_true_path)(void* context, void* object, int argument) noexcept;
};

void FUN_004018F0_SetServices(const FUN_004018F0_Services* services) noexcept;
void FUN_004018F0(bool flag) noexcept;

} // namespace re5::recovered
