#pragma once
#include "REFix.h"

namespace REFix {
    int pre_update_pitch_yaw(int argc, void** argv, REFrameworkTypeDefinitionHandle* arg_tys, unsigned long long ret_addr);
    int pre_add_rank_point_direct(int argc, void** argv, REFrameworkTypeDefinitionHandle* arg_tys, unsigned long long ret_addr);
    int pre_set_interval_level(int argc, void** argv, REFrameworkTypeDefinitionHandle* arg_tys, unsigned long long ret_addr);
    void post_get_control_magnitude(void** ret_val, REFrameworkTypeDefinitionHandle ret_ty, unsigned long long ret_addr);
}