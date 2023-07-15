#include "Hooks.h"

namespace REFix {
    const float DEFAULT_FOV = 90.0f;
    extern const REF::API::Field* camera_param_field;
    extern const REF::API::Field* field_of_view_field;

    int pre_update_pitch_yaw(int argc, void** argv, REFrameworkTypeDefinitionHandle* arg_tys) {
        REF::API::ManagedObject* const camera_param = camera_param_field->get_data<REF::API::ManagedObject*>(argv[1]);
        const float fov = *(float*)field_of_view_field->get_data_raw(camera_param);
        float* input = (float*)&argv[2];
        *input *= fov;
        *input /= DEFAULT_FOV;
        return REFRAMEWORK_HOOK_CALL_ORIGINAL;
    }
}
