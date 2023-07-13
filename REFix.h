#pragma once
#include <reframework/API.hpp>

namespace REF = reframework;

namespace REFix {
    static void post_hook_null(void** ret_val, REFrameworkTypeDefinitionHandle ret_ty) {}
    bool init(const REFrameworkPluginInitializeParam* param);
}

extern "C" __declspec(dllexport) bool reframework_plugin_initialize(const REFrameworkPluginInitializeParam* param);
extern "C" __declspec(dllexport) void reframework_plugin_required_version(REFrameworkPluginVersion* version);