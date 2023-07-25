#include <filesystem>
#include <libconfig.h++>

#include "REFix.h"
#include "AnimCurveFlattener.h"
#include "AnimCurveStraightener.h"
#include "Undamper.h"
#include "Hooks.h"

namespace fs = std::filesystem;

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
    libconfig::Config config;

    bool check_or_set(const char* name) {
        bool value = true;

        if (!config.lookupValue(name, value)) {
            config.getRoot().add(name, libconfig::Setting::TypeBoolean) = true;
        }

        return value;
    }

    bool init() {
        if (!fs::create_directory(
            fs::path(".\\reframework\\data", fs::path::native_format)
        )) {
            // The data directory exists. Read the config.

            try {
                config.readFile(".\\reframework\\data\\refix_config.txt");
            }
            catch (const libconfig::FileIOException&) {
                REF::API::get()->log_warn("[REFix] Failed to open the config.");
            }
            catch (const libconfig::ParseException& p) {
                REF::API::get()->log_warn("[REFix] Config parse error: %s", p.getError());
            }
        }

        const REF::API::TDB* const tdb = REF::API::get()->tdb();
        const REF::API::VMContext* const context = REF::API::get()->get_vm_context();

        // Get pointers to important types, fields, and methods.

        const REF::API::TypeDefinition* const animation_curve_type = tdb->find_type("via.AnimationCurve");
        get_keys_count = animation_curve_type->find_method("getKeysCount");
        get_keys = animation_curve_type->find_method("getKeys");
        set_keys = animation_curve_type->find_method("setKeys");
        key_frame_type = tdb->find_type("via.KeyFrame");
        const REF::API::Method* const get_camera_controller = tdb->find_method(PREFIX ".camera.CameraSystem", "getCameraController");

        // Get the player camera controller.

        const REF::API::ManagedObject* const camera_system = REF::API::get()->get_managed_singleton(PREFIX ".camera.CameraSystem");
        const REF::API::ManagedObject* const player_camera_controller = get_camera_controller->call<REF::API::ManagedObject*>(context, camera_system, 0);

        if (player_camera_controller == nullptr) {
            REF::API::get()->log_error("[REFix] Call to getCameraController failed.");
            return false;
        }

        REF::API::get()->log_info("[REFix] Player Camera Controller found at %p", player_camera_controller);

        // Get the camera settings.

        const REF::API::ManagedObject* const twirler_camera_settings = *player_camera_controller->get_field<REF::API::ManagedObject*>("TwirlerCameraSettings");
        REF::API::get()->log_info("[REFix] Twirler Camera Settings found at %p", twirler_camera_settings);

        if (check_or_set("disable-input-pitch-scaling")) {
            // Get the speed curves.

            const REF::API::ManagedObject* const normal_speed_curve = *twirler_camera_settings->get_field<REF::API::ManagedObject*>("NormalSpeedCurve");
            REF::API::get()->log_info("[REFix] Normal speed curve found at %p", normal_speed_curve);
            const REF::API::ManagedObject* const hold_speed_curve = *twirler_camera_settings->get_field<REF::API::ManagedObject*>("HoldSpeedCurve");
            REF::API::get()->log_info("[REFix] Hold speed curve found at %p", hold_speed_curve);

            // The camera's yaw speed is scaled based on your pitch by default. This sets the scale to 1.0 for all angles.

            value_field = key_frame_type->find_field("value");
            const AnimationCurveFlattener flattener(1.0f);
            flattener.mutate(normal_speed_curve);
            REF::API::get()->log_info("[REFix] Normal speed curve flattened.");
            flattener.mutate(hold_speed_curve);
            REF::API::get()->log_info("[REFix] Hold speed curve flattened.");
        }

        if (check_or_set("remove-input-damping")) {
            // Straighten the input curve.

            const REF::API::ManagedObject* const input_curve = *twirler_camera_settings->get_field<REF::API::ManagedObject*>("InputCurve");
            REF::API::get()->log_info("[REFix] Input curve found at %p", input_curve);
            in_normal_field = key_frame_type->find_field("inNormal");
            out_normal_field = key_frame_type->find_field("outNormal");
            const AnimationCurveStraightener straightener;
            straightener.mutate(input_curve);
            REF::API::get()->log_info("[REFix] Input curve straightened.");

            // Remove input damping.

            const REF::API::TypeDefinition* const damping_struct_single = tdb->find_type(PREFIX ".DampingStruct`1<System.Single>");
            REF::API::ManagedObject* const twirl_speed_yaw = *player_camera_controller->get_field<REF::API::ManagedObject*>("<TwirlSpeedYaw>k__BackingField");
            REF::API::get()->log_info("[REFix] Twirl speed yaw found at %p", twirl_speed_yaw);
            REF::API::ManagedObject* const twirl_speed_pitch = *player_camera_controller->get_field<REF::API::ManagedObject*>("<TwirlSpeedPitch>k__BackingField");
            REF::API::get()->log_info("[REFix] Twirl speed pitch found at %p", twirl_speed_pitch);
            const Undamper undamper(damping_struct_single);
            undamper.undamp(twirl_speed_yaw);
            REF::API::get()->log_info("[REFix] Twirl speed yaw undamped.");
            undamper.undamp(twirl_speed_pitch);
            REF::API::get()->log_info("[REFix] Twirl speed pitch undamped.");
        }

        if (check_or_set("scale-input-with-fov")) {
            // Scale the input by the current FOV.

            camera_param_field = tdb->find_field(PREFIX ".camera.CameraControllerRoot", "<CameraParam>k__BackingField");
            field_of_view_field = tdb->find_field(PREFIX ".CameraParam", "FieldOfView");
            const REF::API::TypeDefinition* const twirler_camera_controller_root_type = tdb->find_type(PREFIX ".camera.TwirlerCameraControllerRoot");
            twirler_camera_controller_root_type->find_method("updatePitch")->add_hook(pre_update_pitch_yaw, post_hook_null, false);
            twirler_camera_controller_root_type->find_method("updateYaw")->add_hook(pre_update_pitch_yaw, post_hook_null, false);
        }

        if (check_or_set("remove-dynamic-difficulty"))
        {
            // Remove dynamic difficulty modulation.

            tdb->find_method(PREFIX ".GameRankSystem", "addRankPointDirect")->add_hook(pre_add_rank_point_direct, post_hook_null, false);
        }

        if (check_or_set("fix-zombie-anims"))
        {
            // Make zombies animate at 60fps.

            tdb->find_method(PREFIX ".MotionIntervalController", "setIntervalLevel")->add_hook(REFix::pre_set_interval_level, REFix::post_hook_null, false);
        }

        // Write the config in case any settings were changed.

        try {
            config.writeFile(".\\reframework\\data\\refix_config.txt");
        }
        catch (libconfig::FileIOException) {
            REF::API::get()->log_warn("[REFix] Could not write to the config.");
        }

        return true;
    }
}

bool reframework_plugin_initialize(const REFrameworkPluginInitializeParam* param) {
    REF::API::initialize(param);
    return REFix::init();
}