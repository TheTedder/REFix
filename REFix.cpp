#include "REFix.h"
#include <memory>
#include <cstdint>

bool reframework_plugin_initialize(const REFrameworkPluginInitializeParam* param) {
    REF::API::initialize(param);
    const std::unique_ptr<REF::API>& api = REF::API::get();
    const REF::API::TDB* const tdb = api->tdb();
    const REF::API::VMContext* const context = api->get_vm_context();

    // Get pointers to important types, fields, and methods.

    const REF::API::Method* const get_camera_controller = tdb->find_method("app.ropeway.camera.CameraSystem", "getCameraController");
    const REF::API::TypeDefinition* const animation_curve_type = tdb->find_type("via.AnimationCurve");
    const REF::API::Method* const get_keys_count = animation_curve_type->find_method("getKeysCount");
    const REF::API::Method* const get_keys = animation_curve_type->find_method("getKeys");
    const REF::API::Method* const set_keys = animation_curve_type->find_method("setKeys");
    const REF::API::TypeDefinition* const key_frame_type = tdb->find_type("via.KeyFrame");
    const REF::API::Field* const value_field = key_frame_type->find_field("value");

    // Get the player camera controller.

    const REF::API::ManagedObject* const camera_system = api->get_managed_singleton("app.ropeway.camera.CameraSystem");
    const REF::API::ManagedObject* const player_camera_controller = get_camera_controller->call<REF::API::ManagedObject*>(context, camera_system, 0);

    if (player_camera_controller == nullptr) {
        api->log_error("[REFix] Call to getCameraController failed.");
        return false;
    }

    api->log_info("[REFix] Player Camera Controller found at %p", player_camera_controller);

    // Get the camera settings.

    const REF::API::ManagedObject* const twirler_camera_settings = *player_camera_controller->get_field<REF::API::ManagedObject*>("TwirlerCameraSettings");
    api->log_info("[REFix] Twirler Camera Settings found at %p", twirler_camera_settings);

    // Get the speed curves.

    const REF::API::ManagedObject* const normal_speed_curve = *twirler_camera_settings->get_field<REF::API::ManagedObject*>("NormalSpeedCurve");
    api->log_info("[REFix] Normal speed curve found at %p", normal_speed_curve);
    const REF::API::ManagedObject* const hold_speed_curve = *twirler_camera_settings->get_field<REF::API::ManagedObject*>("HoldSpeedCurve");
    api->log_info("[REFix] Hold speed curve found at %p", hold_speed_curve);
    REF::API::ManagedObject* const key_frame = key_frame_type->create_instance();
    key_frame->add_ref();

    // The camera speed is scaled based on your pitch by default. This sets the scale to 1.0 for all angles.

    const auto flatten = [get_keys, context, value_field, get_keys_count, set_keys, key_frame](const REF::API::ManagedObject* animation_curve) {
        const uint32_t key_count = get_keys_count->call<uint32_t>(context, animation_curve);

        for (uint32_t i = 0; i < key_count; i++) {
            const REF::API::ManagedObject* const keys_result = get_keys->call<REF::API::ManagedObject*>(key_frame, context, animation_curve, i);

            if (keys_result == 0) {
                REF::API::get()->log_warn("[REFix] Failed to retrieve KeyFrame %u for animation curve.", i);
                continue;
            }

            // Give every KeyFrame a value of 1. This turns the curve into a flat line.
            
            float* const value = (float*)value_field->get_data_raw(key_frame, true);
            *value = 1.0f;
            set_keys->call(context, animation_curve, i, key_frame);
        }
    };

    if (key_frame->get_ref_count() > 0) {
        key_frame->release();
    }

    flatten(normal_speed_curve);
    flatten(hold_speed_curve);

    return true;
}