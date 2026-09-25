#include "recovered.hpp"
#include "re5/recovered/fun_004022f0.hpp"
#include <array>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#if defined(__unix__) || defined(__APPLE__)
#include <sys/mman.h>
#include <unistd.h>
#endif

using namespace re5::recovered;
namespace {
unsigned long long checks = 0;
void check(bool ok, const char* description) {
    ++checks;
    if (!ok) throw std::runtime_error(description);
}
// Check both the attachment reconstruction and the repository's host adapter.
// The adapter is not asserted to have the original x86 layout/calling convention.
void check_compare(const RawStringObject& object, const byte* argument,
                   s32 expected, const char* description) {
    check(object.FUN_004022F0(argument)==expected, description);
    FUN_004022F0_Object view{nullptr, object.storage == nullptr ? nullptr :
        reinterpret_cast<const char*>(object.storage+8)};
    check(FUN_004022F0(object.storage == nullptr ? nullptr : &view,
                      reinterpret_cast<const char*>(argument))==expected,
          "canonical comparator must agree with exact-target reference");
}
s32 oracle(const byte* argument, const byte* stored) {
    // Independent, single-byte reference: unsigned lexicographic comparison.
    while (*argument == *stored && *argument != 0) { ++argument; ++stored; }
    return (*argument > *stored) - (*argument < *stored);
}
void write_le32(byte* p, u32 value) {
    for (unsigned i=0;i<4;++i) p[i]=static_cast<byte>(value>>(8*i));
}
u32 next_random(u32& state) {
    state ^= state<<13u; state ^= state>>17u; state ^= state<<5u; return state;
}
void run_semantics() {
    std::array<byte,10> storage{};
    std::array<byte,2> argument{};
    RawStringObject object{storage.data()};
    for (unsigned s=0;s<256;++s) {
        storage[8]=static_cast<byte>(s);
        for (unsigned a=0;a<256;++a) {
            argument[0]=static_cast<byte>(a);
            check_compare(object,argument.data(),oracle(argument.data(),storage.data()+8),
                  "all first-byte pairs including high-bit values and NUL");
        }
    }
    RawStringObject empty{nullptr};
    for (unsigned a=0;a<256;++a) {
        argument[0]=static_cast<byte>(a);
        check_compare(empty,argument.data(),(a==0?0:1),"null backing guard");
    }
    const std::array<const char*,11> words={"", "a", "aa", "ab", "abc", "abcd", "abcdefg", "abcdefgh", "longer text", "x", "z"};
    for (const char* s:words) {
        std::vector<byte> block(8+std::strlen(s)+1,0);
        std::memcpy(block.data()+8,s,std::strlen(s)+1);
        // Deliberately inconsistent cached length: compare must not consult it.
        write_le32(block.data()+4,0xDEADBEEFu);
        RawStringObject current{block.data()};
        for (const char* a:words) {
            const auto* arg=reinterpret_cast<const byte*>(a);
            check_compare(current,arg,oracle(arg,block.data()+8),"different lengths and parity");
        }
        check_compare(current,block.data()+8,0,"argument aliases stored string");
        check(current.FUN_00402420()==block.data()+8,"accessor returns original storage + 8");
    }
    u32 seed=0x323d1aab;
    for (unsigned n=0;n<4096;++n) {
        std::vector<byte> block(8+34,0), arg(34,0);
        const unsigned length=next_random(seed)%33;
        for (unsigned i=0;i<length;++i) {
            block[8+i]=static_cast<byte>(1+next_random(seed)%255);
            arg[i]=block[8+i];
        }
        RawStringObject current{block.data()};
        if (length && (n&1u)) arg[next_random(seed)%length]=static_cast<byte>(next_random(seed));
        check_compare(current,arg.data(),oracle(arg.data(),block.data()+8),"seeded multi-byte comparison");
    }
    for (u32 value : {0u,1u,127u,128u,0x7fffffffu,0x80000000u,0xffffffffu}) {
        write_le32(storage.data()+4,value);
        check(object.FUN_00402350()==value,"length exact little-endian u32");
    }
    check(empty.FUN_00402350()==0,"null storage length");
    const byte* fallback=empty.FUN_00402420();
    check(fallback!=nullptr && fallback[0]==0,"relocated fallback contains NUL");
    check(empty.FUN_00402420()==fallback,"fallback pointer identity stable");
    RawStringObject another_empty{nullptr};
    check(another_empty.FUN_00402420()==fallback,"fallback shared across instances");
    struct Guarded { byte before[8]; FourByteObject object; byte after[8]; } guarded{};
    std::memset(guarded.before,0xa5,sizeof guarded.before);
    std::memset(guarded.after,0x5a,sizeof guarded.after);
    for (unsigned n=0;n<4096;++n) {
        const u32 a=next_random(seed),b=next_random(seed),c=next_random(seed),d=next_random(seed);
        auto* result=guarded.object.FUN_004022A0(a,b,c,d);
        check(result==&guarded.object,"byte writer returns this");
        check(guarded.object.fields[0]==static_cast<byte>(c) &&
              guarded.object.fields[1]==static_cast<byte>(b) &&
              guarded.object.fields[2]==static_cast<byte>(a) &&
              guarded.object.fields[3]==static_cast<byte>(d),"byte order and upper-bit truncation");
        for (byte v:guarded.before) check(v==0xa5,"no write before object");
        for (byte v:guarded.after) check(v==0x5a,"no write after object");
    }
    std::cout<<"PASS semantic checks="<<checks<<"\n";
}
#if defined(__unix__) || defined(__APPLE__)
struct GuardPages {
    byte* base=nullptr;
    std::size_t size=0;
    GuardPages() {
        const long page=sysconf(_SC_PAGESIZE);
        if(page<=0) throw std::runtime_error("cannot determine page size");
        size=static_cast<std::size_t>(page);
        void* p=mmap(nullptr,2*size,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
        if(p==MAP_FAILED) throw std::runtime_error("mmap failed");
        base=static_cast<byte*>(p);
        if(mprotect(base+size,size,PROT_NONE)!=0) {
            munmap(base,2*size); base=nullptr;
            throw std::runtime_error("mprotect failed");
        }
    }
    GuardPages(const GuardPages&)=delete;
    ~GuardPages() {if(base) munmap(base,2*size);}
    byte* tail(std::size_t n) {return base+size-n;}
};
void run_guards() {
    GuardPages left,right;
    RawStringObject empty{nullptr};
    byte* arg=left.tail(1); *arg=0;
    check_compare(empty,arg,0,"null backing: NUL is last readable byte");
    *arg=0xff;
    check_compare(empty,arg,1,"null backing: mismatch does not read beyond first byte");
    for (std::size_t length=0;length<=8;++length) {
        byte* a=left.tail(length+1); byte* block=right.tail(8+length+1);
        std::memset(block,0,8+length+1);
        for(std::size_t i=0;i<length;++i) {a[i]=static_cast<byte>('a'+i);block[8+i]=a[i];}
        a[length]=0;
        RawStringObject current{block};
        check_compare(current,a,0,"NUL at guard page, both loop exits and parity");
    }
    byte* block=right.tail(9);
    std::memset(block,0,9); block[8]='b'; *left.tail(1)='a';
    RawStringObject current{block};
    check_compare(current,left.tail(1),-1,"first-byte mismatch: neither buffer needs readable terminator");
    block=right.tail(8); std::memset(block,0,8);write_le32(block+4,0xF1E2D3C4u);
    current.storage=block;
    check(current.FUN_00402350()==0xF1E2D3C4u,"length reads header only, text page inaccessible");
    check(current.FUN_00402420()==right.base+right.size,"accessor returns text address without dereference");
    std::cout<<"PASS guard-page checks="<<checks<<"\n";
}
#else
void run_guards() {throw std::runtime_error("guard page tests require POSIX in this baseline");}
#endif
} // namespace
int main(int argc,char** argv) {
    try {
        if(argc==2 && std::string(argv[1])=="--guards") run_guards();
        else run_semantics();
        return 0;
    } catch(const std::exception& e) {
        std::cerr<<"FAIL after "<<checks<<" checks: "<<e.what()<<"\n";
        return 1;
    }
}
