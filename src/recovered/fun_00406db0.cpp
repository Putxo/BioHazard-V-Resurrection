#include "re5/recovered/fun_00406db0.hpp"
namespace re5::recovered {
namespace {
const FUN_00406DB0_Services* g_services=nullptr;
struct Group{const char* name;std::uintptr_t table;std::int32_t count;};
constexpr Group k_before_messages[]={
{"Image\\Archive\\DlcResource",0x01567EA0U,1},
{"Image\\Archive\\Title2Resource",0x015677C8U,2},
{"Image\\Archive\\Shop2Resource",0x01567940U,3},
};
constexpr Group k_after_messages[]={
{"Image\\Archive\\Menu2Resource",0x015678B0U,2},
{"Image\\Archive\\Extra1Resource",0x015679E0U,20},
{"Image\\Archive\\Extra2Resource",0x01567A80U,18},
{"Image\\Archive\\Extra1End",0x01567B10U,2},
{"Image\\Archive\\Extra2End",0x01567B20U,2},
{"Image\\Archive\\Merce2Resource",0x01567B30U,13},
{"Image\\Archive\\FigureResource",0x01567BC8U,7},
{"Image\\Archive\\Credit2Resource",0x01567C08U,1},
};
constexpr const char* k_begin=">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
constexpr const char* k_end="<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";
void dispatch(const FUN_00406DB0_Services& s,const Group& g) noexcept{
 s.dispatch_resource_group(s.context,-1,g.name,g.table,g.count,0U,0);
}
bool complete(const FUN_00406DB0_Services& s) noexcept {
 return s.dispatch_resource_group&&s.trace_marker&&s.load_message_group&&s.load_game2_resource&&s.scan_resource_tree;
}
}
void FUN_00406DB0_SetServices(const FUN_00406DB0_Services* s) noexcept{g_services=s;}
void FUN_00406DB0() noexcept{
 const auto* s=g_services;
 if(!s||!complete(*s)) return;
 dispatch(*s,k_before_messages[0]);
 s->trace_marker(s->context,k_begin);
 dispatch(*s,k_before_messages[1]);
 dispatch(*s,k_before_messages[2]);
 for(std::uint32_t i=0;i<11U;++i) s->load_message_group(s->context,i);
 for(const auto& g:k_after_messages) dispatch(*s,g);
 s->load_game2_resource(s->context);
 s->trace_marker(s->context,k_begin);
 s->scan_resource_tree(s->context,"sound\\bgm_dc\\oto","rSoundSourceMusic",".sngw");
 s->trace_marker(s->context,k_end);
}
}
