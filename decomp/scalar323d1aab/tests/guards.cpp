#include "re5/scalar_catalog.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/mman.h>
#include <unistd.h>
using namespace re5::scalar323d1aab;
int main(){
 const auto page=static_cast<std::size_t>(sysconf(_SC_PAGESIZE));unsigned bodies=0,checks=0;
 for(unsigned i=0;i<entry_count;++i){const auto& e=entries[i];unsigned width=0;bool read=false;
  switch(e.kind){
   case Kind::load8:width=1;read=true;break;
   case Kind::bitfield8:case Kind::bitfield32:width=4;read=true;break;
   case Kind::store_arg8:case Kind::store_imm8:width=1;break;
   case Kind::store_arg16:width=2;break;
   case Kind::store_arg32:case Kind::store_imm32:width=4;break;
   default:continue;
  }
  ++bodies;const std::size_t usable=((std::size_t(e.offset)+width+page-1)/page)*page;
  const std::size_t size=usable+2*page;
  auto* mem=static_cast<u8*>(mmap(nullptr,size,PROT_NONE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0));
  if(mem==MAP_FAILED || mprotect(mem+page,usable,PROT_READ|PROT_WRITE)!=0)return 2;
  auto* end=mem+page+usable;auto* slot=end-width;void* self=slot-e.offset;
  for(unsigned n=0;n<4;++n){
   if(mprotect(mem+page,usable,PROT_READ|PROT_WRITE)!=0)return 2;
   const u32 value=n==0?0U:n==1?0xffffffffU:n==2?0x80000000U:0xEFCDAB89U;
   std::memcpy(slot,&value,width);
   if(read && mprotect(mem+page,usable,PROT_READ)!=0)return 2;
   u32 expected=value;u32 result=0;
   switch(e.kind){
    case Kind::load8:result=e.g8(self);expected=u8(value);break;
    case Kind::bitfield8:result=e.g8(self);expected=u8((value>>(e.shift&31U))&e.imm);break;
    case Kind::bitfield32:result=e.g32(self);expected=(value>>(e.shift&31U))&e.imm;break;
    case Kind::store_arg8:e.w8(self,u8(value));break;
    case Kind::store_arg16:e.w16(self,u16(value));break;
    case Kind::store_arg32:e.w32(self,value);break;
    case Kind::store_imm8:case Kind::store_imm32:e.store(self);expected=e.imm;break;
    default:return 3;
   }
   if(!read){std::memcpy(&result,slot,width);if(width==1)expected=u8(expected);if(width==2)expected=u16(expected);}
   if(result!=expected){std::fprintf(stderr,"guard result mismatch %08X\n",e.va);return 1;}++checks;
  }
  if(munmap(mem,size)!=0)return 2;
 }
 std::printf("PASS %u memory bodies; %u guarded boundary/read-only checks\n",bodies,checks);
}
