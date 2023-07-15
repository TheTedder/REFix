#include "InputScaling.h"

namespace REFix {
    const float DEFAULT_FOV = 90.0f;
    const REF::API::Field* InputScaling::camera_param_field = nullptr;
    const REF::API::Field* InputScaling::field_of_view_field = nullptr;

    InputScaling::UpdatePitch::UpdatePitch(
        const REF::API::TypeDefinition* twirler_camera_controller_root
    ) : SimpleHook(
        twirler_camera_controller_root->find_method("updatePitch"),
        pre_update_pitch_yaw
    ) {}

    InputScaling::UpdateYaw::UpdateYaw(
        const REF::API::TypeDefinition* twirler_camera_controller_root
    ) : SimpleHook(
        twirler_camera_controller_root->find_method("updateYaw"), pre_update_pitch_yaw
    ) {}

    InputScaling::InputScaling() : update_pitch(
        REF::API::get()->tdb()->find_type(
            "app.ropeway.camera.TwirlerCameraControllerRoot"
        )
    ),
        update_yaw(
            REF::API::get()->tdb()->find_type(
                "app.ropeway.camera.TwirlerCameraControllerRoot"
            )
        )
    {
        const REF::API::TDB* const tdb = REF::API::get()->tdb();

        if (camera_param_field == nullptr) {
            camera_param_field = tdb->find_field(
                "app.ropeway.camera.CameraControllerRoot",
                "<CameraParam>k__BackingField"
            );
        }

        if (field_of_view_field == nullptr) {
            field_of_view_field = tdb->find_field(
                "app.ropeway.CameraParam", "FieldOfView"
            );
        }
    }

    int InputScaling::pre_update_pitch_yaw(
        int argc, void** argv, REFrameworkTypeDefinitionHandle* arg_tys
    ) {
        REF::API::ManagedObject* const camera_param = camera_param_field->get_data<REF::API::ManagedObject*>(argv[1]);
        const float fov = *(float*)field_of_view_field->get_data_raw(camera_param);
        float* input = (float*)&argv[2];
        *input *= fov;
        *input /= DEFAULT_FOV;
        return REFRAMEWORK_HOOK_CALL_ORIGINAL;
    }

    bool InputScaling::enable() {
        return this->update_pitch.enable() && this->update_yaw.enable();
    }

    bool InputScaling::disable() {
        return this->update_pitch.enable() && this->update_yaw.enable();
    }
}