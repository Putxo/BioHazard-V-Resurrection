// Executes ONLY exactly reconstructed two-instruction fragments from the local
// input, in x86-64 mode with a low-32-bit ECX data address. This is NOT an IA32
// process/game launch and does not prove the original 32-bit return-stack ABI.
#include "re5/catalog.hpp"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iterator>
#include <vector>
#include <sys/mman.h>
#include <unistd.h>
using namespace re5::leaf323d1aab;
#if !defined(__x86_64__) || !defined(__linux__)
#error This optional fragment test requires Linux x86-64 and MAP_32BIT.
#endif
namespace {
using Native = u32 (__attribute__((ms_abi)) *)(const void*);
unsigned long long checks=0;
[[noreturn]] void fatal(const char* message) { std::fprintf(stderr,"FAIL: %s\n",message); std::exit(1); }
std::vector<u8> encoding(const Entry& e) {
    std::vector<u8> out;
    auto dword=[&out](u32 x) { for (unsigned i=0;i<4U;++i) out.push_back(u8(x>>(8U*i))); };
    if (e.kind==Kind::constant) { out.push_back(0xB8); dword(e.immediate); }
    else if (e.kind==Kind::zero) { out.push_back(e.encoding ? 0x31 : 0x33); out.push_back(0xC0); }
    else if (e.kind==Kind::identity) { out={0x8B,0xC1}; }
    else {
        switch (e.kind) {
        case Kind::load32: out={0x8B}; break;
        case Kind::address: out={0x8D}; break;
        case Kind::load8: out={0x0F,0xB6}; break;
        case Kind::load16: out={0x0F,0xB7}; break;
        case Kind::sign8: out={0x0F,0xBE}; break;
        case Kind::sign16: out={0x0F,0xBF}; break;
        default: fatal("unsupported family");
        }
        if (e.encoding==0U) out.push_back(0x01);
        else if (e.encoding==8U) { out.push_back(0x41); out.push_back(u8(e.displacement)); }
        else if (e.encoding==32U) { out.push_back(0x81); dword(e.displacement); }
        else fatal("unsupported displacement encoding");
    }
    out.push_back(0xC3);
    if (out.size()!=e.size) fatal("descriptor length mismatch");
    return out;
}
void compare(const Entry& e, Native original, const void* self) {
    const u32 first=original(self);
    const u32 second=e.function(self);
    ++checks;
    if (first!=second) {
        std::fprintf(stderr,"Mismatch VA=%08X original=%08X source=%08X\n",e.va,first,second);
        std::exit(1);
    }
}
}
int main(int argc, char** argv) {
    if (argc!=2) fatal("use tools/verify_local.py --exe; the Python entry point also verifies the full SHA-256");
    std::ifstream file(argv[1],std::ios::binary);
    if (!file) fatal("cannot read input");
    const std::vector<u8> binary((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());
    if (binary.size()!=19977216U) fatal("wrong input size");
    const auto page=static_cast<std::size_t>(sysconf(_SC_PAGESIZE));
    if (page<4096U) fatal("unexpected page size");
    auto align=[page](std::size_t n) { return (n+page-1U)/page*page; };
    const std::size_t code_size=align((entry_count+1U)*16U);
    auto* code=static_cast<u8*>(mmap(nullptr,code_size,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0));
    if (code==MAP_FAILED) fatal("cannot map fragment storage");
    // Verify every body before making any copied byte executable.
    for (unsigned i=0;i<entry_count;++i) {
        const auto& e=entries[i];
        const auto expected=encoding(e);
        if (std::size_t(e.file_offset)+e.size>binary.size()
                || std::memcmp(binary.data()+e.file_offset,expected.data(),e.size)!=0)
            fatal("input range does not exactly match the safe instruction whitelist");
        std::memcpy(code+(i+1U)*16U,binary.data()+e.file_offset,e.size);
    }
    if (mprotect(code,code_size,PROT_READ|PROT_EXEC)!=0) fatal("cannot protect copied code RX");
    u32 max_offset=0;
    for (const auto& e:entries) max_offset=std::max(max_offset,e.displacement);
    const std::size_t accessible=align(max_offset+64U);
    auto* memory=static_cast<u8*>(mmap(nullptr,accessible+page,PROT_READ|PROT_WRITE,
                                      MAP_PRIVATE|MAP_ANONYMOUS|MAP_32BIT,-1,0));
    if (memory==MAP_FAILED || reinterpret_cast<uptr>(memory)+accessible+page>0xFFFFFFFFULL)
        fatal("cannot establish a low-32-bit address window");
    if (mprotect(memory+accessible,page,PROT_NONE)!=0) fatal("cannot create guard page");
    const u32 edges[]={0U,1U,0x7FU,0x80U,0xFFU,0x100U,0x7FFFU,0x8000U,0xFFFFU,
                       0x10000U,0x7FFFFFFFU,0x80000000U,0xFFFFFFFEU,0xFFFFFFFFU};
    std::vector<u8> expected_memory(accessible);
    unsigned guarded_routines=0;
    for (unsigned i=0;i<entry_count;++i) {
        const auto& e=entries[i];
        const auto original=reinterpret_cast<Native>(code+(i+1U)*16U);
        if (!e.width) {
            for (u32 token:edges) compare(e,original,reinterpret_cast<const void*>(uptr(token)));
            compare(e,original,memory);
            continue;
        }
        ++guarded_routines;
        std::memset(memory,0xA5,accessible);
        auto* field=memory+accessible-e.width;
        auto* self=field-e.displacement;
        auto run=[&](u32 value) {
            for (unsigned j=0;j<e.width;++j) field[j]=u8(value>>(8U*j));
            compare(e,original,self);
        };
        if (e.width<=2U) {
            const u32 limit=e.width==1U ? 256U : 65536U;
            for (u32 value=0;value<limit;++value) run(value);
        } else {
            u32 state=e.va;
            for (unsigned trial=0;trial<1024U;++trial) { state=state*1664525U+1013904223U; run(state); }
        }
        for (u32 value:edges) run(value);
        std::fill(expected_memory.begin(),expected_memory.end(),0xA5);
        for (unsigned j=0;j<e.width;++j) expected_memory[accessible-e.width+j]=0xFF;
        if (std::memcmp(memory,expected_memory.data(),accessible)!=0) fatal("a leaf unexpectedly modified data");
    }
    munmap(memory,accessible+page);
    munmap(code,code_size);
    std::printf("{\"test\":\"original_fragment_comparison\",\"routines\":%u,\"checks\":%llu,\"guarded_load_routines\":%u,\"execution_mode\":\"x86-64 compatible fragments with low32 data\",\"ia32_process_executed\":false,\"game_executed\":false,\"status\":\"PASS\"}\n",entry_count,checks,guarded_routines);
}
