#include "ZombieFix.h"

namespace REFix {
    ZombieFix::ZombieFix() : SimpleHook(REF::API::get()->tdb()->find_method("app.ropeway.MotionIntervalController", "setIntervalLevel"), pre_set_interval_level) {}

    int ZombieFix::pre_set_interval_level(int argc, void** argv, REFrameworkTypeDefinitionHandle* arg_tys) {
        int32_t* level = (int32_t*)&argv[2];
        *level = 0;
        return REFRAMEWORK_HOOK_CALL_ORIGINAL;
    }
}