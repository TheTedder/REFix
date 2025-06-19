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

#define TDB() REF::API::get()->tdb()

#define VMC() REF::API::get()->get_vm_context()

#define LOG_INFO(s, ...) REF::API::get()->log_info("[REFix] " s __VA_OPT__(,) __VA_ARGS__)
#define LOG_WARN(s, ...) REF::API::get()->log_warn("[REFix] " s __VA_OPT__(,) __VA_ARGS__)
#define LOG_ERROR(s, ...) REF::API::get()->log_error("[REFix] " s __VA_OPT__(,) __VA_ARGS__)

// Print a pointer location to the log.
#define PRINT_PTR(ptr) LOG_INFO(#ptr " found at %p", (ptr))

namespace REFix {
    static int pre_hook_null(int argc, void** argv, REFrameworkTypeDefinitionHandle* arg_tys, unsigned long long ret_addr)
    {
        return REFRAMEWORK_HOOK_CALL_ORIGINAL;
    }

    static void post_hook_null(void** ret_val, REFrameworkTypeDefinitionHandle ret_ty, unsigned long long ret_addr) {}
}

extern "C" __declspec(dllexport) bool reframework_plugin_initialize(const REFrameworkPluginInitializeParam* param);
extern "C" __declspec(dllexport) void reframework_plugin_required_version(REFrameworkPluginVersion* version);