#pragma once
#include "REFix.h"

namespace REFix {
    void post_hook_null(void** ret_val, REFrameworkTypeDefinitionHandle ret_ty);
    int pre_update_pitch_yaw(int argc, void** argv, REFrameworkTypeDefinitionHandle* arg_tys);
    int pre_add_rank_point_direct(int argc, void** argv, REFrameworkTypeDefinitionHandle* arg_tys);
}