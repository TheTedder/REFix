#pragma once
#include "SimpleHook.h"

namespace REFix {
    class FixRank : public SimpleHook {
    public:
        FixRank();
        
        const char* display_name() const override {
            return "Remove Dynamic Difficulty";
        }

        const char* internal_name() const override {
            return "Fix_Rank";
        }

        static int pre_add_rank_point_direct(int argc, void** argv, REFrameworkTypeDefinitionHandle* arg_tys);
    };
}