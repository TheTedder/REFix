#pragma once
#include <reframework/API.hpp>

#ifdef RE2
#define PREFIX "app.ropeway"

#if TDB_VERSION == 66
#define GAME "RE2_TDB66"
#elif TDB_VERSION == 70
#define GAME "RE2"
#endif

#elif defined(RE3)
#define PREFIX "offline"

#if TDB_VERSION == 67
#define GAME "RE3_TDB67"
#elif TDB_VERSION == 70
#define GAME "RE3"
#endif

#endif

namespace REF = reframework;

namespace REFix {
    static void post_hook_null(void** ret_val, REFrameworkTypeDefinitionHandle ret_ty) {}
}

extern "C" __declspec(dllexport) bool reframework_plugin_initialize(const REFrameworkPluginInitializeParam* param);
extern "C" __declspec(dllexport) void reframework_plugin_required_version(REFrameworkPluginVersion* version);