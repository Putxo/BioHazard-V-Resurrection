// Source-only read-boundary tests; no proprietary code, no executable mappings.
#include "re5/catalog.hpp"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/mman.h>
#include <unistd.h>
using namespace re5::leaf323d1aab;
namespace {
[[noreturn]] void fail(const char* what) { std::fprintf(stderr,"FAIL: %s\n",what); std::exit(1); }
u32 expected(const Entry& e,u32 value) {
    if (e.width==1) value &= 0xFFU;
    if (e.width==2) value &= 0xFFFFU;
    if (e.kind==Kind::sign8 && (value&0x80U)) value |= 0xFFFFFF00U;
    if (e.kind==Kind::sign16 && (value&0x8000U)) value |= 0xFFFF0000U;
    return value;
}
}
int main() {
    const long queried=sysconf(_SC_PAGESIZE);
    if (queried<=0) fail("page size");
    const auto page=static_cast<unsigned long>(queried);
    u32 maximum=0;
    for (const auto& e:entries) maximum=std::max(maximum,e.displacement);
    const unsigned long accessible=((maximum+16UL+page-1UL)/page)*page;
    auto* memory=static_cast<u8*>(mmap(nullptr,accessible+page,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0));
    if (memory==MAP_FAILED) fail("mmap");
    if (mprotect(memory+accessible,page,PROT_NONE)) fail("guard page");
    unsigned routines=0,checks=0;
    for (const auto& e:entries) {
        if (!e.width) continue;
        ++routines;
        auto* field=memory+accessible-e.width;
        auto* self=field-e.displacement;
        for (const u32 value:{0U,0x80U,0x8000U,0xFFFFFFFFU}) {
            if (mprotect(memory,accessible,PROT_READ|PROT_WRITE)) fail("writable data");
            for (unsigned j=0;j<e.width;++j) field[j]=u8(value>>(8U*j));
            if (mprotect(memory,accessible,PROT_READ)) fail("read-only data");
            if (e.function(self)!=expected(e,value)) fail("boundary value");
            ++checks;
        }
    }
    if (munmap(memory,accessible+page)) fail("munmap");
    if (routines!=598U) fail("coverage changed");
    std::printf("{\"test\":\"source_guard_pages\",\"routines\":%u,\"checks\":%u,\"status\":\"PASS\"}\n",routines,checks);
}
