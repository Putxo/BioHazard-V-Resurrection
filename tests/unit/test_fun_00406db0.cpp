#include "re5/recovered/fun_00406db0.hpp"
#include <cassert>
#include <cstdint>
#include <string>
#include <vector>
namespace {
struct State{std::vector<std::string> e;};
void d(void* c,std::int32_t m,const char* n,std::uintptr_t t,std::int32_t dc,std::uintptr_t rt,std::int32_t rc) noexcept{
 auto&s=*static_cast<State*>(c); s.e.push_back("d:"+std::to_string(m)+":"+n+":"+std::to_string(t)+":"+std::to_string(dc)+":"+std::to_string(rt)+":"+std::to_string(rc));
}
void tr(void*c,const char*m) noexcept{static_cast<State*>(c)->e.push_back(std::string("t:")+m);}
void msg(void*c,std::uint32_t i) noexcept{static_cast<State*>(c)->e.push_back("m:"+std::to_string(i));}
void g2(void*c) noexcept{static_cast<State*>(c)->e.push_back("g2");}
void scan(void*c,const char*a,const char*b,const char*d0) noexcept{static_cast<State*>(c)->e.push_back(std::string("s:")+a+":"+b+":"+d0);}
re5::recovered::FUN_00406DB0_Services mk(State&s){return {&s,d,tr,msg,g2,scan};}
}
void test_fun_00406db0(){
 using namespace re5::recovered;
 FUN_00406DB0_SetServices(nullptr); FUN_00406DB0();
 State inc{}; auto is=mk(inc); is.load_game2_resource=nullptr; FUN_00406DB0_SetServices(&is); FUN_00406DB0(); assert(inc.e.empty());
 State s{}; auto sv=mk(s); FUN_00406DB0_SetServices(&sv); FUN_00406DB0();
 assert(s.e.size()==27U);
 assert(s.e[0]=="d:-1:Image\\Archive\\DlcResource:22445728:1:0:0");
 assert(s.e[1].rfind("t:>>>>>>>>",0)==0);
 assert(s.e[2]=="d:-1:Image\\Archive\\Title2Resource:22443976:2:0:0");
 assert(s.e[3]=="d:-1:Image\\Archive\\Shop2Resource:22444352:3:0:0");
 for(unsigned i=0;i<11;++i) assert(s.e[4+i]=="m:"+std::to_string(i));
 assert(s.e[15]=="d:-1:Image\\Archive\\Menu2Resource:22444208:2:0:0");
 assert(s.e[16]=="d:-1:Image\\Archive\\Extra1Resource:22444512:20:0:0");
 assert(s.e[17]=="d:-1:Image\\Archive\\Extra2Resource:22444672:18:0:0");
 assert(s.e[18]=="d:-1:Image\\Archive\\Extra1End:22444816:2:0:0");
 assert(s.e[19]=="d:-1:Image\\Archive\\Extra2End:22444832:2:0:0");
 assert(s.e[20]=="d:-1:Image\\Archive\\Merce2Resource:22444848:13:0:0");
 assert(s.e[21]=="d:-1:Image\\Archive\\FigureResource:22445000:7:0:0");
 assert(s.e[22]=="d:-1:Image\\Archive\\Credit2Resource:22445064:1:0:0");
 assert(s.e[23]=="g2");
 assert(s.e[24].rfind("t:>>>>>>>>",0)==0);
 assert(s.e[25]=="s:sound\\bgm_dc\\oto:rSoundSourceMusic:.sngw");
 assert(s.e[26].rfind("t:<<<<<<<<",0)==0);
 FUN_00406DB0_SetServices(nullptr);
}
