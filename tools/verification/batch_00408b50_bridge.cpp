#include "re5/recovered/batch_00408b50.hpp"
using namespace re5::recovered;
extern "C" Batch408_U32 re5_batch_408_run(Batch408_U32 va, Batch408_Access* a,
                                        Batch408_U32 self, Batch408_U32 argument) {
    switch (va) {
    case 0x00408B50U: return FUN_00408B50(*a, self);
    case 0x00408B90U: return FUN_00408B90(*a, self) ? 1U : 0U;
    case 0x00408BC0U: FUN_00408BC0(*a, self); return 0U;
    case 0x00408BE0U: FUN_00408BE0(*a, self); return 0U;
    case 0x00408C10U: return FUN_00408C10(*a, self);
    case 0x00408C30U: return FUN_00408C30(*a, self);
    case 0x00408C50U: return FUN_00408C50(*a, self);
    case 0x00408C70U: return FUN_00408C70(*a, self);
    case 0x00408CA0U: return FUN_00408CA0(*a, self);
    case 0x00408CD0U: return FUN_00408CD0(*a, self, argument);
    case 0x00408CF0U: FUN_00408CF0(*a, argument); return 0U;
    case 0x00408DD0U: FUN_00408DD0(*a); return 0U;
    case 0x00408DE0U: return FUN_00408DE0(*a, self);
    case 0x00408E00U: FUN_00408E00(*a, self); return 0U;
    default: return 0xBAD00408U;
    }
}
