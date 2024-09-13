#include <filesystem>
#include <thread>
#include <chrono>
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
    const REF::API::TypeDefinition* damping_struct_single;
    libconfig::Config config;

    static bool check_or_set(const char* name) {
        bool value = true;

        if (!config.lookupValue(name, value)) {
            config.getRoot().add(name, libconfig::Setting::TypeBoolean) = true;
        }

        return value;
    }

    static void disable_input_pitch_scaling(const reframework::API::ManagedObject* twirler_camera_settings)
    {
        // Get the speed curves.

        const REF::API::ManagedObject* const normal_speed_curve = *twirler_camera_settings->get_field<REF::API::ManagedObject*>("NormalSpeedCurve");
        PRINT_PTR(normal_speed_curve);
        const REF::API::ManagedObject* const hold_speed_curve = *twirler_camera_settings->get_field<REF::API::ManagedObject*>("HoldSpeedCurve");
        PRINT_PTR(hold_speed_curve);

        // The camera's yaw speed is scaled based on your pitch by default. This sets the scale to 1.0 for all angles.
        
        const AnimationCurveFlattener flattener(1.0f);
        flattener.mutate(normal_speed_curve);
        LOG_INFO("Normal speed curve flattened.");
        flattener.mutate(hold_speed_curve);
        LOG_INFO("Hold speed curve flattened.");
    }

    static void remove_input_damping(
        const reframework::API::ManagedObject* twirler_camera_settings,
        const reframework::API::ManagedObject* camera_controller
    )
    {
        // Straighten the input curve.

        const REF::API::ManagedObject* const input_curve = *twirler_camera_settings->get_field<REF::API::ManagedObject*>("InputCurve");
        PRINT_PTR(input_curve);
        const AnimationCurveStraightener straightener;
        straightener.mutate(input_curve);
        LOG_INFO("Input curve straightened.");

        // Remove input damping.

        REF::API::ManagedObject* const twirl_speed_yaw = *camera_controller->get_field<REF::API::ManagedObject*>(
            "<TwirlSpeedYaw>k__BackingField"
        );

        PRINT_PTR(twirl_speed_yaw);

        REF::API::ManagedObject* const twirl_speed_pitch = *camera_controller->get_field<REF::API::ManagedObject*>(
            "<TwirlSpeedPitch>k__BackingField"
        );

        PRINT_PTR(twirl_speed_pitch);
        const Undamper undamper(damping_struct_single);
        undamper.undamp(twirl_speed_yaw);
        LOG_INFO("Twirl speed yaw undamped.");
        undamper.undamp(twirl_speed_pitch);
        LOG_INFO("Twirl speed pitch undamped.");
    }

    static bool init() {
        if (!fs::create_directory(
            fs::path(".\\reframework\\data", fs::path::native_format)
        )) {
            // The data directory exists. Read the config.

            try {
                config.readFile(".\\reframework\\data\\refix_config.txt");
            }
            catch (const libconfig::FileIOException&) {
                LOG_WARN("Failed to open the config.");
            }
            catch (const libconfig::ParseException& p) {
                LOG_WARN("Config parse error: %s", p.getError());
            }
        }
        
        // Get pointers to important types, fields, and methods.

        const REF::API::TypeDefinition* const animation_curve_type = TDB()->find_type("via.AnimationCurve");
        get_keys_count = animation_curve_type->find_method("getKeysCount");
        PRINT_PTR(get_keys_count);
        get_keys = animation_curve_type->find_method("getKeys");
        PRINT_PTR(get_keys);
        set_keys = animation_curve_type->find_method("setKeys");
        PRINT_PTR(set_keys);
        key_frame_type = TDB()->find_type("via.KeyFrame");
        PRINT_PTR(key_frame_type);
        const REF::API::Method* const get_camera_controller = TDB()->find_method(PREFIX ".camera.CameraSystem", "getCameraController");

        // Get the player camera controller.

        const REF::API::ManagedObject* const camera_system = REF::API::get()->get_managed_singleton(PREFIX ".camera.CameraSystem");

        if (camera_system == nullptr)
        {
            LOG_ERROR("Could not get Camera System singleton.");
            return false;
        }

        PRINT_PTR(camera_system);
        const REF::API::ManagedObject* const player_camera_controller = get_camera_controller->call<REF::API::ManagedObject*>(VMC(), camera_system, 0);

        if (player_camera_controller == nullptr) {
            LOG_ERROR("Call to getCameraController(0) failed.");
            return false;
        }

        PRINT_PTR(player_camera_controller);

#ifdef RE3
        const REF::API::ManagedObject* const player_sight_camera_controller = get_camera_controller->call<REF::API::ManagedObject*>(VMC(), camera_system, 1);

        if (player_sight_camera_controller == nullptr) {
            LOG_ERROR("Call to getCameraController(1) failed.");
            return false;
        }

        PRINT_PTR(player_sight_camera_controller);
#endif

        // Get the camera settings.

        const REF::API::ManagedObject* const twirler_camera_settings = *player_camera_controller->get_field<REF::API::ManagedObject*>("TwirlerCameraSettings");
        PRINT_PTR(twirler_camera_settings);

#ifdef RE3
        const REF::API::ManagedObject* const sight_twirler_camera_settings = *player_sight_camera_controller->get_field<REF::API::ManagedObject*>("TwirlerCameraSettings");
        PRINT_PTR(sight_twirler_camera_settings);
#endif

        if (check_or_set("disable-input-pitch-scaling")) {
            value_field = key_frame_type->find_field("value");
            PRINT_PTR(value_field);
            disable_input_pitch_scaling(twirler_camera_settings);

#ifdef RE3
            disable_input_pitch_scaling(sight_twirler_camera_settings);
#endif
        }

        if (check_or_set("remove-input-damping")) {
            in_normal_field = key_frame_type->find_field("inNormal");
            PRINT_PTR(in_normal_field);
            out_normal_field = key_frame_type->find_field("outNormal");
            PRINT_PTR(out_normal_field);
            damping_struct_single = TDB()->find_type(PREFIX ".DampingStruct`1<System.Single>");
            PRINT_PTR(damping_struct_single);
            remove_input_damping(twirler_camera_settings, player_camera_controller);

#ifdef RE3
            remove_input_damping(sight_twirler_camera_settings, player_sight_camera_controller);
#endif
        }

        if (check_or_set("scale-input-with-fov")) {
            // Scale the input by the current FOV.

            camera_param_field = TDB()->find_field(PREFIX ".camera.CameraControllerRoot", "<CameraParam>k__BackingField");
            PRINT_PTR(camera_param_field);
            field_of_view_field = TDB()->find_field(PREFIX ".CameraParam", "FieldOfView");
            PRINT_PTR(field_of_view_field);
            const REF::API::TypeDefinition* const twirler_camera_controller_root_type = TDB()->find_type(PREFIX ".camera.TwirlerCameraControllerRoot");
            twirler_camera_controller_root_type->find_method("updatePitch")->add_hook(pre_update_pitch_yaw, post_hook_null, false);
            twirler_camera_controller_root_type->find_method("updateYaw")->add_hook(pre_update_pitch_yaw, post_hook_null, false);
        }

        if (check_or_set("remove-dynamic-difficulty"))
        {
            // Remove dynamic difficulty modulation.

            TDB()->find_method(PREFIX ".GameRankSystem", "addRankPointDirect")->add_hook(pre_add_rank_point_direct, post_hook_null, false);
        }

        if (check_or_set("fix-zombie-anims"))
        {
            // Make zombies animate at 60fps.

            TDB()->find_method(PREFIX ".MotionIntervalController", "setIntervalLevel")->add_hook(REFix::pre_set_interval_level, REFix::post_hook_null, false);
        }

        // Write the config in case any settings were changed.

        try {
            config.writeFile(".\\reframework\\data\\refix_config.txt");
        }
        catch (const libconfig::FileIOException&) {
            LOG_WARN("Could not write to the config.");
        }

        return true;
    }
}

bool reframework_plugin_initialize(const REFrameworkPluginInitializeParam* param) {
    REF::API::initialize(param);
    LOG_INFO("Running setup...");

    while (!REFix::init())
    {
        LOG_INFO("Setup failed. Retrying...");
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    LOG_INFO("Setup completed.");
    return true;
}