#pragma once
#include "SimpleHook.h"

namespace REFix {
    class ZombieFix : public SimpleHook {
    public:
        ZombieFix();

        const char* display_name() const override {
            return "Zombie Fix";
        }

        const char* internal_name() const override {
            return "Zombiefix";
        }

        static int pre_set_interval_level(int argc, void** argv, REFrameworkTypeDefinitionHandle* arg_tys);
    };
}