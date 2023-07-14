#include "ZombieFix.h"

namespace REFix {
    ZombieFix::ZombieFix() : set_interval_level(REF::API::get()->tdb()->find_method("app.ropeway.MotionIntervalController", "setIntervalLevel")) {
        this->hook_id = 0;
    }

    bool ZombieFix::enable() {
        this->hook_id = this->set_interval_level->add_hook(pre_set_interval_level, post_hook_null, false);
        REF::API::get()->log_info("set_interval_level hooked with id %u", this->hook_id);
        return true;
    }

    bool ZombieFix::disable() {
        this->set_interval_level->remove_hook(this->hook_id);
        return true;
    }

    int ZombieFix::pre_set_interval_level(int argc, void** argv, REFrameworkTypeDefinitionHandle* arg_tys) {
        int32_t* level = (int32_t*)&argv[2];
        *level = 0;
        return REFRAMEWORK_HOOK_CALL_ORIGINAL;
    }
}