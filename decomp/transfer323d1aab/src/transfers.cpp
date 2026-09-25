#include "transfers.hpp"
namespace re5::transfer323d1aab {
namespace {
template<unsigned W> u32 load(const void* p,u32 offset) noexcept {
    static_assert(W==2 || W==4);
    const auto* b=static_cast<const u8*>(p)+offset;
    u32 v=u32(b[0])|(u32(b[1])<<8U);
    if constexpr(W==4)v|=(u32(b[2])<<16U)|(u32(b[3])<<24U);
    return v;
}
template<unsigned W> void store(void* p,u32 offset,u32 v) noexcept {
    static_assert(W==2 || W==4);
    auto* b=static_cast<u8*>(p)+offset;
#if defined(__GNUC__) || defined(__clang__)
    __builtin_memcpy(b,&v,W);
#else
    b[0]=u8(v);b[1]=u8(v>>8U);
    if constexpr(W==4){b[2]=u8(v>>16U);b[3]=u8(v>>24U);}
#endif
}
void* address(u32 p) noexcept {return reinterpret_cast<void*>(uptr(p));}
void mark(void* self,u32 f,u32 m) noexcept {store<4>(self,f,load<4>(self,f)|m);}
template<unsigned W> void pair(const void* self,void* output,u32 a,u32 b,u32 c,u32 d) noexcept {
    // Both input values are captured before either output write.
    const u32 first=load<W>(self,a),second=load<W>(self,b);
    store<W>(output,c,first);store<W>(output,d,second);
}
}
#define RE5_STORE(n,d) \
void RE5_TRANSFER_CC n(void* self,u32 bits) noexcept {store<4>(self,d,bits);}
#define RE5_SNAPSHOT(n,s,f,m,d) \
void RE5_TRANSFER_CC n(void* self,u32 bits) noexcept { \
    const u32 saved=load<4>(self,s);mark(self,f,m);store<4>(self,d,bits);store<4>(self,s,saved); \
}
#define RE5_DIRTY(n,f,m,d) \
void RE5_TRANSFER_CC n(void* self,u32 bits) noexcept {mark(self,f,m);store<4>(self,d,bits);}
#define RE5_INDIRECT(n,a,d) \
void RE5_TRANSFER_CC n(void* self,u32 bits) noexcept { \
    const u32 p=load<4>(self,a);store<4>(address(u32(p+d)),0,bits); \
}
#define RE5_DUPLICATE(n,a,b) \
void RE5_TRANSFER_CC n(void* self,u32 bits) noexcept {store<4>(self,a,bits);store<4>(self,b,bits);}
#define RE5_DUP_ZERO(n,a,b,z) \
void RE5_TRANSFER_CC n(void* self,u32 bits) noexcept { \
    store<4>(self,a,bits);store<4>(self,b,bits);store<4>(self,z,0U); \
}
#define RE5_STORE_FLAG(n,d,f,v) \
void RE5_TRANSFER_CC n(void* self,u32 bits) noexcept {store<4>(self,d,bits);static_cast<u8*>(self)[f]=u8(v);}
#define RE5_PAIR16(n,a,b,c,d) \
void RE5_TRANSFER_CC n(const void* self,void* out) noexcept {pair<2>(self,out,a,b,c,d);}
#define RE5_PAIR32(n,a,b,c,d) \
void RE5_TRANSFER_CC n(const void* self,void* out) noexcept {pair<4>(self,out,a,b,c,d);}
#define RE5_NEST16(n,t,i,c,e,a,b,d,f,x,y) \
void RE5_TRANSFER_CC n(const void* self,void* out) noexcept { \
    const u32 owner=load<4>(self,t); \
    if(owner!=0U){ \
        const u32 index=load<4>(self,i); \
        if((index&0x80000000U)==0U){ \
            const u32 count=load<4>(address(u32(owner+c)),0); \
            if((count&0x80000000U)==0U && index<count){ \
                const u32 node=load<4>(address(u32(owner+index*4U+e)),0); \
                if(node!=0U){pair<2>(address(node),out,a,b,d,f);return;} \
            } \
        } \
    } \
    store<2>(out,d,x);store<2>(out,f,y); \
}
#include "records.inc"
#undef RE5_STORE
#undef RE5_SNAPSHOT
#undef RE5_DIRTY
#undef RE5_INDIRECT
#undef RE5_DUPLICATE
#undef RE5_DUP_ZERO
#undef RE5_STORE_FLAG
#undef RE5_PAIR16
#undef RE5_PAIR32
#undef RE5_NEST16
}
