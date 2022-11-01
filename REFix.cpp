#include "REFix.h"
#include <memory>
#include <cstdint>

bool reframework_plugin_initialize(const REFrameworkPluginInitializeParam* param) {
    REF::API::initialize(param);
    const std::unique_ptr<REF::API>& api = REF::API::get();
    const REF::API::TDB* tdb = api->tdb();
    const REF::API::VMContext* context = api->get_vm_context();

    // Get the player camera controller.

    const REF::API::ManagedObject* camera_system = api->get_managed_singleton("app.ropeway.camera.CameraSystem");
    const REF::API::Method* get_camera_controller = tdb->find_method("app.ropeway.camera.CameraSystem", "getCameraController");
    const REF::API::ManagedObject* player_camera_controller = get_camera_controller->call<REF::API::ManagedObject*>(context, camera_system, 0);

    if (player_camera_controller == nullptr) {
        api->log_error("[REFix] Call to getCameraController failed.");
        return false;
    }

    api->log_info("[REFix] Player Camera Controller found at %p", player_camera_controller);

    // Get the camera settings.

    const REF::API::ManagedObject* twirler_camera_settings = *player_camera_controller->get_field<REF::API::ManagedObject*>("TwirlerCameraSettings");
    api->log_info("[REFix] Twirler Camera Settings found at %p", twirler_camera_settings);
    return true;
}