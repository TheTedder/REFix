#include "Module.h"

namespace REFix {
    bool enable_with_error(Module* m) {
        const bool result = m->enable();

        if (!result) {
            REF::API::get()->log_error("[REFix] Failed to enable module %s", m->display_name());
        }

        return result;
    }

    bool disable_with_error(Module* m) {
        const bool result = m->disable();

        if (!result) {
            REF::API::get()->log_error("[REFix] Failed to disable module %s", m->display_name());
        }

        return result;
    }
}