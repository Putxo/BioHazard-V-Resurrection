// Exercise the thirty existing field readers selected for canonical registration.
#include "re5/leaves.hpp"
#if !defined(RE5_CANONICAL_I386)
#include <sys/mman.h>
#include <unistd.h>
#endif
using namespace re5::leaf323d1aab;
namespace {
u32 checks=0, seed=0x37AC190BU;
const char* current="setup";
u32 length(const char* text){u32 n=0;while(text[n])++n;return n;}
#if defined(RE5_CANONICAL_I386)
static_assert(sizeof(void*)==4);
u32 sys3(u32 number,u32 a,u32 b,u32 c){u32 result;asm volatile("int $0x80":"=a"(result):"0"(number),"b"(a),"c"(b),"d"(c):"memory");return result;}
void output(const char* text){sys3(4,1,u32(uptr(text)),length(text));}
[[noreturn]] void quit(u32 code){sys3(1,code,0,0);__builtin_unreachable();}
void* allocate(u32 bytes){u32 arguments[]={0,bytes,3,0x22,0xffffffffU,0};u32 value=sys3(90,u32(uptr(arguments)),0,0);return value>=0xfffff001U?nullptr:reinterpret_cast<void*>(uptr(value));}
bool protect(void* p,u32 bytes,u32 mode){return sys3(125,u32(uptr(p)),bytes,mode)==0;}
void release(void* p,u32 bytes){sys3(91,u32(uptr(p)),bytes,0);}
#else
void output(const char* text){const auto ignored=write(1,text,length(text));(void)ignored;}
[[noreturn]] void quit(u32 code){_exit(int(code));}
void* allocate(u32 bytes){void* p=mmap(nullptr,bytes,3,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);return p==MAP_FAILED?nullptr:p;}
bool protect(void* p,u32 bytes,u32 mode){return mprotect(p,bytes,int(mode))==0;}
void release(void* p,u32 bytes){munmap(p,bytes);}
#endif
void number(u32 value){char text[12];u32 i=11;text[i]=0;do{text[--i]=char('0'+value%10U);value/=10U;}while(value);output(text+i);}
void require(bool value){++checks;if(!value){output("FAIL ");output(current);output(" check ");number(checks);output("\n");quit(1);}}
u32 random_word(){seed^=seed<<13U;seed^=seed>>17U;seed^=seed<<5U;return seed;}
void put(u8* p,u32 value,u32 width){for(u32 i=0;i<width;++i)p[i]=u8(value>>(8*i));}
u32 read(const u8* p,u32 width){u32 value=0;for(u32 i=0;i<width;++i)value|=u32(p[i])<<(8*i);return value;}
struct Case{const char* name;Function function;u32 offset,width;};
#if defined(RE5_CANONICAL_REFERENCE)
extern "C" {
#define CANONICAL_LEAF(n,o,w) u32 RE5_LEAF_CC original_##n(const void*) noexcept;
#include "canonical_200_leaves.inc"
#undef CANONICAL_LEAF
}
#endif
#define CANONICAL_LEAF(n,o,w) {#n,&n,o,w},
const Case recovered[]={
#include "canonical_200_leaves.inc"
};
#undef CANONICAL_LEAF
#if defined(RE5_CANONICAL_REFERENCE)
#define CANONICAL_LEAF(n,o,w) {#n,&original_##n,o,w},
const Case originals[]={
#include "canonical_200_leaves.inc"
};
#undef CANONICAL_LEAF
#endif
static_assert(sizeof(recovered)/sizeof(Case)==30);
struct Region{
    static constexpr u32 bytes=0x40000,page=4096;
    u8* data;
    Region():data(static_cast<u8*>(allocate(bytes))){require(data!=nullptr);}
    ~Region(){release(data,bytes);}
    Region(const Region&)=delete;Region& operator=(const Region&)=delete;
};
void properties(const Case* cases){
    Region object;
    for(u32 i=0;i<Region::bytes;++i)object.data[i]=u8(i*29U^(i>>4U));
    for(u32 n=0;n<30;++n){const Case& c=cases[n];current=c.name;
        for(u32 alignment=0;alignment<8;++alignment){u8* self=object.data+16+alignment;u8* field=self+c.offset;
            for(u32 j=0;j<1024;++j){
                const u32 input=(random_word()&0xffffff00U)|(j&255U);const u32 expected=c.width==1?(input&255U):input;
                put(field,input,c.width);const u8 left=field[-1],right=field[c.width];
                require(c.function(self)==expected);
                require(read(field,c.width)==expected && field[-1]==left && field[c.width]==right);
            }
        }
    }
}
void guards(const Case* cases){
    for(u32 n=0;n<30;++n){const Case& c=cases[n];current=c.name;Region object;
        u8* page=object.data+Region::bytes-2*Region::page;
        // All other pages are inaccessible; the readable page is read-only.
        require(protect(object.data,Region::bytes,0));require(protect(page,Region::page,3));
        u8* last=page+Region::page-c.width;put(last,0xFEDCBA98U,c.width);
        require(protect(page,Region::page,1));
        require(c.function(last-c.offset)==(c.width==1?0x98U:0xFEDCBA98U));
        require(protect(page,Region::page,3));put(page,0x80FF017FU,c.width);require(protect(page,Region::page,1));
        require(c.function(page-c.offset)==(c.width==1?0x7FU:0x80FF017FU));
    }
}
void run(const Case* cases,const char* label,bool p,bool g){checks=0;seed=0x37AC190BU;if(p)properties(cases);if(g)guards(cases);output("PASS ");output(label);output(" ");number(checks);output(" checks; 30 bodies\n");}
}
#if defined(RE5_CANONICAL_I386)
extern "C" void canonical_entry(){
    run(recovered,"i386-source",true,true);
#if defined(RE5_CANONICAL_REFERENCE)
    run(originals,"i386-hash-checked-reference",true,true);
#endif
}
asm(".text\n.global _start\n_start:\n xorl %ebp,%ebp\n andl $-16,%esp\n call canonical_entry\n xorl %ebx,%ebx\n movl $1,%eax\n int $0x80\n.section .note.GNU-stack,\"\",@progbits\n");
#else
int main(int argc,char** argv){if(argc!=2)return 2;const bool p=argv[1][0]=='p',g=argv[1][0]=='g';if(!p&&!g)return 2;run(recovered,p?"properties":"guards",p,g);return 0;}
#endif
