#include "mutations.hpp"
namespace re5::mutator323d1aab {
namespace {
u32 load(const void* self,u32 offset) noexcept {
    const auto* b=static_cast<const u8*>(self)+offset;
    return u32(b[0])|(u32(b[1])<<8U)|(u32(b[2])<<16U)|(u32(b[3])<<24U);
}
template<unsigned Width> void store(void* self,u32 offset,u32 value) noexcept {
    static_assert(Width==1 || Width==2 || Width==4);
    auto* b=static_cast<u8*>(self)+offset;
#if defined(__GNUC__) || defined(__clang__)
    __builtin_memcpy(b,&value,Width);
#else
    b[0]=u8(value);
    if constexpr(Width>=2) b[1]=u8(value>>8U);
    if constexpr(Width==4) { b[2]=u8(value>>16U); b[3]=u8(value>>24U); }
#endif
}
void mark(void* self,u32 offset,u32 bits) noexcept {
    store<4>(self,offset,load(self,offset)|bits);
}
}
#define SNAPSHOT(n,w,s,f,m,d) \
void RE5_MUT_CC n(void* self,u32 value) noexcept { \
    const u32 saved=load(self,s);mark(self,f,m); \
    store<w>(self,d,value);store<4>(self,s,saved); \
}
#define DIRECT(n,w,f,m,d) \
void RE5_MUT_CC n(void* self,u32 value) noexcept { \
    mark(self,f,m);store<w>(self,d,value); \
}
#define RE5_S8(n,...) SNAPSHOT(n,1,__VA_ARGS__)
#define RE5_S16(n,...) SNAPSHOT(n,2,__VA_ARGS__)
#define RE5_S32(n,...) SNAPSHOT(n,4,__VA_ARGS__)
#define RE5_D8(n,...) DIRECT(n,1,__VA_ARGS__)
#define RE5_D16(n,...) DIRECT(n,2,__VA_ARGS__)
#define RE5_D32(n,...) DIRECT(n,4,__VA_ARGS__)
// Read source+4 after the first write: the source can overlap the destination.
#define RE5_PAIR(n,a,b,s,f,m) \
void RE5_MUT_CC n(void* self,const void* source) noexcept { \
    const u32 first=load(source,0);store<4>(self,a,first); \
    const u32 second=load(source,4);const u32 saved=load(self,s); \
    mark(self,f,m);store<4>(self,b,second);store<4>(self,s,saved); \
}
// The second pair is reread, not a reuse of the first pair's initial values.
#define RE5_DUAL(n,a,b,s,f,m,c,d,t,g,k) \
void RE5_MUT_CC n(void* self,const void* source) noexcept { \
    const u32 first=load(source,0);store<4>(self,a,first); \
    const u32 second=load(source,4);mark(self,f,m);store<4>(self,b,second); \
    const u32 saved0=load(self,s);store<4>(self,s,saved0); \
    const u32 third=load(source,0);store<4>(self,c,third); \
    const u32 fourth=load(source,4);const u32 saved1=load(self,t); \
    mark(self,g,k);store<4>(self,d,fourth);store<4>(self,t,saved1); \
}
#define RE5_COPY(n,s,f,m,d) \
void RE5_MUT_CC n(void* self,const void* source) noexcept { \
    const u32 value=load(source,0);const u32 saved=load(self,s); \
    mark(self,f,m);store<4>(self,d,value);store<4>(self,s,saved); \
}
#define RE5_SHIFT(n,s,f,m,d,h,k) \
void RE5_MUT_CC n(void* self,u32 value) noexcept { \
    const u32 saved=load(self,s);mark(self,f,m); \
    u32 difference=(value<<h)^load(self,d);store<4>(self,s,saved); \
    difference&=k;store<4>(self,d,load(self,d)^difference); \
}
#define RE5_MASK(n,s,f,m,d,k) \
void RE5_MUT_CC n(void* self,u32 value) noexcept { \
    u32 difference=load(self,d)^value;const u32 saved=load(self,s); \
    mark(self,f,m);difference&=k;store<4>(self,d,load(self,d)^difference); \
    store<4>(self,s,saved); \
}
#include "records.inc"
#undef SNAPSHOT
#undef DIRECT
#undef RE5_S8
#undef RE5_S16
#undef RE5_S32
#undef RE5_D8
#undef RE5_D16
#undef RE5_D32
#undef RE5_PAIR
#undef RE5_DUAL
#undef RE5_COPY
#undef RE5_SHIFT
#undef RE5_MASK
}
