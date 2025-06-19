#include <cstdint>
#include "Hooks.h"
#include "Enums.h"

namespace REFix {
    static const float DEFAULT_FOV = 90.0f;
    extern const REF::API::Field* camera_param_field;
    extern const REF::API::Field* field_of_view_field;
    extern REF::API::ManagedObject* input_system;
    extern const REF::API::Field* input_system_input_mode;

    int pre_update_pitch_yaw(int argc, void** argv, REFrameworkTypeDefinitionHandle* arg_tys, unsigned long long ret_addr) {
        REF::API::ManagedObject* const camera_param = camera_param_field->get_data<REF::API::ManagedObject*>(argv[1]);
        const float fov = field_of_view_field->get_data<float>(camera_param);
        float* input = (float*)&argv[2];
        *input *= fov;
        *input /= DEFAULT_FOV;
        return REFRAMEWORK_HOOK_CALL_ORIGINAL;
    }

    int pre_add_rank_point_direct(int argc, void** argv, REFrameworkTypeDefinitionHandle* arg_tys, unsigned long long ret_addr)
    {
        const RankPointType rankType = *(RankPointType*)&argv[3];

        if (rankType == RankPointType::FromFsm ||
            rankType == RankPointType::FromScript ||
            rankType == RankPointType::FromDirectSet ||
            rankType == RankPointType::FromDebug) {
            return REFRAMEWORK_HOOK_CALL_ORIGINAL;
        }

        return REFRAMEWORK_HOOK_SKIP_ORIGINAL;
    }
    
    int pre_set_interval_level(int argc, void** argv, REFrameworkTypeDefinitionHandle* arg_tys, unsigned long long ret_addr)
    {
        int32_t* level = (int32_t*)&argv[2];
        *level = 0;
        return REFRAMEWORK_HOOK_CALL_ORIGINAL;
    }

    void post_get_control_magnitude(void** ret_val, REFrameworkTypeDefinitionHandle ret_ty, unsigned long long ret_addr)
    {
        const InputMode mode = input_system_input_mode->get_data<InputMode>(input_system);
        float* magnitude = (float*)ret_val;

        if (mode == InputMode::MouseKeyboard && *magnitude > 0.0f)
        {
            *magnitude = 1.0f;
        }
    }
}
