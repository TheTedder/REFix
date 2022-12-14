#include "init.h"
#include "AnimCurveFlattener.h"
#include "AnimCurveStraightener.h"
#include "Undamper.h"
#include "Hooks.h"

namespace REFix {
    const REF::API::Method* get_keys_count;
    const REF::API::Method* get_keys;
    const REF::API::Method* set_keys;
    const REF::API::TypeDefinition* key_frame_type;
    const REF::API::Field* value_field;
    const REF::API::Field* in_normal_field;
    const REF::API::Field* out_normal_field;
    const REF::API::Field* camera_param_field;
    const REF::API::Field* field_of_view_field;
}

bool reframework_plugin_initialize(const REFrameworkPluginInitializeParam* param) {
    REF::API::initialize(param);
    const REF::API::TDB* const tdb = REF::API::get()->tdb();
    const REF::API::VMContext* const context = REF::API::get()->get_vm_context();

    // Get pointers to important types, fields, and methods.

    const REF::API::TypeDefinition* const animation_curve_type = tdb->find_type("via.AnimationCurve");
    REFix::get_keys_count = animation_curve_type->find_method("getKeysCount");
    REFix::get_keys = animation_curve_type->find_method("getKeys");
    REFix::set_keys = animation_curve_type->find_method("setKeys");
    REFix::key_frame_type = tdb->find_type("via.KeyFrame");
    REFix::value_field = REFix::key_frame_type->find_field("value");
    REFix::in_normal_field = REFix::key_frame_type->find_field("inNormal");
    REFix::out_normal_field = REFix::key_frame_type->find_field("outNormal");
    REFix::camera_param_field = tdb->find_field("app.ropeway.camera.CameraControllerRoot", "<CameraParam>k__BackingField");
    REFix::field_of_view_field = tdb->find_field("app.ropeway.CameraParam", "FieldOfView");
    const REF::API::Method* const get_camera_controller = tdb->find_method("app.ropeway.camera.CameraSystem", "getCameraController");
    const REF::API::TypeDefinition* const damping_struct_single = tdb->find_type("app.ropeway.DampingStruct`1<System.Single>");
    const REF::API::TypeDefinition* const twirler_camera_controller_root_type = tdb->find_type("app.ropeway.camera.TwirlerCameraControllerRoot");

    // Get the player camera controller.

    const REF::API::ManagedObject* const camera_system = REF::API::get()->get_managed_singleton("app.ropeway.camera.CameraSystem");
    const REF::API::ManagedObject* const player_camera_controller = get_camera_controller->call<REF::API::ManagedObject*>(context, camera_system, 0);

    if (player_camera_controller == nullptr) {
        REF::API::get()->log_error("[REFix] Call to getCameraController failed.");
        return false;
    }

    REF::API::get()->log_info("[REFix] Player Camera Controller found at %p", player_camera_controller);

    // Get the camera settings.

    const REF::API::ManagedObject* const twirler_camera_settings = *player_camera_controller->get_field<REF::API::ManagedObject*>("TwirlerCameraSettings");
    REF::API::get()->log_info("[REFix] Twirler Camera Settings found at %p", twirler_camera_settings);

    // Get the speed curves.

    const REF::API::ManagedObject* const normal_speed_curve = *twirler_camera_settings->get_field<REF::API::ManagedObject*>("NormalSpeedCurve");
    REF::API::get()->log_info("[REFix] Normal speed curve found at %p", normal_speed_curve);
    const REF::API::ManagedObject* const hold_speed_curve = *twirler_camera_settings->get_field<REF::API::ManagedObject*>("HoldSpeedCurve");
    REF::API::get()->log_info("[REFix] Hold speed curve found at %p", hold_speed_curve);

    // The camera's yaw speed is scaled based on your pitch by default. This sets the scale to 1.0 for all angles.

    const REFix::AnimationCurveFlattener flattener(1.0f);
    flattener.mutate(normal_speed_curve);
    REF::API::get()->log_info("[REFix] Normal speed curve flattened.");
    flattener.mutate(hold_speed_curve);
    REF::API::get()->log_info("[REFix] Hold speed curve flattened.");

    // Straighten the input curve.

    const REF::API::ManagedObject* const input_curve = *twirler_camera_settings->get_field<REF::API::ManagedObject*>("InputCurve");
    REF::API::get()->log_info("[REFix] Input curve found at %p", input_curve);
    const REFix::AnimationCurveStraightener straightener;
    straightener.mutate(input_curve);
    REF::API::get()->log_info("[REFix] Input curve straightened.");

    // Remove input damping.

    REF::API::ManagedObject* const twirl_speed_yaw = *player_camera_controller->get_field<REF::API::ManagedObject*>("<TwirlSpeedYaw>k__BackingField");
    REF::API::get()->log_info("[REFix] Twirl speed yaw found at %p", twirl_speed_yaw);
    REF::API::ManagedObject* const twirl_speed_pitch = *player_camera_controller->get_field<REF::API::ManagedObject*>("<TwirlSpeedPitch>k__BackingField");
    REF::API::get()->log_info("[REFix] Twirl speed pitch found at %p", twirl_speed_pitch);
    const REFix::Undamper undamper(damping_struct_single);
    undamper.undamp(twirl_speed_yaw);
    REF::API::get()->log_info("[REFix] Twirl speed yaw undamped.");
    undamper.undamp(twirl_speed_pitch);
    REF::API::get()->log_info("[REFix] Twirl speed pitch undamped.");

    // Scale the input by the current FOV.

    twirler_camera_controller_root_type->find_method("updatePitch")->add_hook(REFix::pre_update_pitch_yaw, REFix::post_hook_null, false);
    twirler_camera_controller_root_type->find_method("updateYaw")->add_hook(REFix::pre_update_pitch_yaw, REFix::post_hook_null, false);

    // Remove dynamic difficulty modulation.

    tdb->find_method("app.ropeway.GameRankSystem", "addRankPointDirect")->add_hook(REFix::pre_add_rank_point_direct, REFix::post_hook_null, false);
    
    // Make zombies animate at 60fps.

    tdb->find_method("app.ropeway.MotionIntervalController", "setIntervalLevel")->add_hook(REFix::pre_set_interval_level, REFix::post_hook_null, false);
    
    return true;
}