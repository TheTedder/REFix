#pragma once
#include "Module.h"

namespace REFix {
    class ZombieFix : public Module {
    public:
        ZombieFix();
        bool enable() override;
        bool disable() override;

        const char* display_name() const override {
            return "Zombie Fix";
        }

        const char* internal_name() const override {
            return "zombiefix";
        }

        static int pre_set_interval_level(int argc, void** argv, REFrameworkTypeDefinitionHandle* arg_tys);

    private:
        const REF::API::Method* const set_interval_level;
        unsigned int hook_id;
    };
}