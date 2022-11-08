#pragma once
#include "REFix.h"

namespace REFix {
    int pre_update_pitch_yaw(int argc, void** argv, REFrameworkTypeDefinitionHandle* arg_tys);
    void post_update_pitch_yaw(void** ret_val, REFrameworkTypeDefinitionHandle ret_ty);
}