#include "framework.h"

namespace REF = reframework;

extern "C" __declspec(dllexport) bool reframework_plugin_initialize(const REFrameworkPluginInitializeParam* param) {
    REF::API::initialize(param);
    auto& api = REF::API::get();
    const REF::API::TDB* tbd = api->tdb();
    REF::API::VMContext* context = api->get_vm_context();

    // Plugin functionality goes here.

    return true;
}