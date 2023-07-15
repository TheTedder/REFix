#pragma once
#include "Module.h"

namespace REFix {
    class SimpleHook : public Module {
    public:
        SimpleHook(const REF::API::Method* method, REFPreHookFn pre);
        bool enable() override;
        bool disable() override;
        const char* display_name() const = 0;
        const char* internal_name() const = 0;

    protected:
        const REF::API::Method* const hooked;
        REFPreHookFn const prehook;
        unsigned int hook_id = 0;
    };
}