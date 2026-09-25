#include "re5/scalar_catalog.hpp"
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <set>
#include <vector>
#include <sys/mman.h>
#include <unistd.h>
using namespace re5::scalar323d1aab;
namespace {
unsigned long long checks=0;u32 rng=0x715F2D49U;
void check(bool ok,const Entry& e,const char* why){++checks;if(!ok){std::fprintf(stderr,"FAIL %08X: %s\n",e.va,why);std::abort();}}
u32 next(){rng^=rng<<13U;rng^=rng>>17U;rng^=rng<<5U;return rng;}
u32 read_word(const void* p){u32 x=0;std::memcpy(&x,p,4);return x;}
void write_word(void* p,u32 x){std::memcpy(p,&x,4);}
void* ptr(u32 x){return reinterpret_cast<void*>(static_cast<std::uintptr_t>(x));}
struct GlobalPages {
 std::vector<void*> mapped;std::size_t page=static_cast<std::size_t>(sysconf(_SC_PAGESIZE));
 GlobalPages(){
  std::set<std::uintptr_t> pages;
  for(unsigned i=0;i<entry_count;++i)for(u32 a:{entries[i].src,entries[i].dst})if(a){pages.insert(a&~(page-1));pages.insert((a+3)&~(page-1));}
  for(auto a:pages){
   void* p=mmap(reinterpret_cast<void*>(a),page,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS|MAP_FIXED_NOREPLACE,-1,0);
   if(p==MAP_FAILED){std::perror("fixed test page unavailable (not overwritten)");std::exit(2);}mapped.push_back(p);
  }
 }
 ~GlobalPages(){for(void* p:mapped)munmap(p,page);}
};
}
int main(){
 GlobalPages globals;u32 maximum=0;for(unsigned i=0;i<entry_count;++i)maximum=std::max(maximum,entries[i].offset);
 std::vector<u8> storage(static_cast<std::size_t>(maximum)+64,0xA5);void* self=storage.data()+16;
 for(unsigned i=0;i<entry_count;++i){const Entry& e=entries[i];
  check(e.va%16==0,e,"aligned unique body");
  if(e.kind==Kind::noop){e.action();check(true,e,"no-op returned");continue;}
  for(unsigned n=0;n<64;++n){
   const u32 x=n==0?0U:n==1?0xffffffffU:n==2?0x80000000U:n==3?0x80U:next();
   const u32 argument=next();auto* p=static_cast<u8*>(self)+e.offset;
   std::fill(p-4,p+8,u8(0xA5));write_word(p,x);u8 expected[12];std::memcpy(expected,p-4,12);
   switch(e.kind){
    case Kind::zero8:check(e.g8(nullptr)==0,e,"zero AL");break;
    case Kind::constant8:check(e.g8(nullptr)==e.imm,e,"constant AL");break;
    case Kind::load8:check(e.g8(self)==u8(x),e,"byte load");break;
    case Kind::bitfield8:check(e.g8(self)==u8((x>>(e.shift&31U))&e.imm),e,"masked AL result");break;
    case Kind::bitfield32:check(e.g32(self)==((x>>(e.shift&31U))&e.imm),e,"masked EAX result");break;
    case Kind::store_arg8:e.w8(self,u8(argument));expected[4]=u8(argument);break;
    case Kind::store_arg16:{e.w16(self,u16(argument));u16 value=u16(argument);std::memcpy(expected+4,&value,2);break;}
    case Kind::store_arg32:e.w32(self,argument);std::memcpy(expected+4,&argument,4);break;
    case Kind::store_imm8:e.store(self);expected[4]=u8(e.imm);break;
    case Kind::store_imm32:e.store(self);std::memcpy(expected+4,&e.imm,4);break;
    case Kind::global_store32:{
     write_word(ptr(e.dst),x);u8* dest=static_cast<u8*>(ptr(e.dst));
     // Absolute target DWORD may sit at either page boundary; only access its own 4 bytes.
     e.action();check(read_word(dest)==e.imm,e,"fixed global store");break;
    }
    case Kind::global_copy32:{
     write_word(ptr(e.dst),argument);write_word(ptr(e.src),x);
     check(e.copy()==x,e,"global copy EAX");check(read_word(ptr(e.dst))==x,e,"global destination");check(read_word(ptr(e.src))==x,e,"global source unchanged");break;
    }
    case Kind::noop:std::abort();
   }
   check(std::memcmp(p-4,expected,12)==0,e,"only expected object bytes changed");
  }
 }
 std::printf("PASS %u scalar bodies; %llu property checks\n",entry_count,checks);
}
