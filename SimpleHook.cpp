#include "SimpleHook.h"

namespace REFix {
    SimpleHook::SimpleHook(const REF::API::Method* method, REFPreHookFn pre) : hooked(method), prehook(pre) {}

    bool SimpleHook::enable()
    {
        this->hook_id = hooked->add_hook(prehook, post_hook_null, false);
        return true;
    }

    bool SimpleHook::disable()
    {
        this->hooked->remove_hook(this->hook_id);
        return true;
    }
}