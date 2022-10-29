#include "framework.h"

namespace REF = reframework;

extern "C" __declspec(dllexport) bool reframework_plugin_initialize(const REFrameworkPluginInitializeParam* param) {
    REF::API::initialize(param);
    const std::unique_ptr<REF::API>& api = REF::API::get();
    const REF::API::TDB* tdb = api->tdb();
    const REF::API::VMContext* context = api->get_vm_context();

    // Plugin functionality goes here.

    return true;
}