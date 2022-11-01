#pragma once
#include <reframework/API.hpp>

namespace REF = reframework;

extern "C" __declspec(dllexport) bool reframework_plugin_initialize(const REFrameworkPluginInitializeParam* param);

template <typename T, typename I>
inline T deref(I* location, int offset) {
    return  *((T*)(location + offset));
}