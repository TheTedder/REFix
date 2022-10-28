#pragma once
#include <sdkddkver.h>

#define NTDDI_VERSION NTDDI_WIN10
#define _WIN32_WINNT _WIN32_WINNT_WIN10
#define WINVER _WIN32_WINNT_WIN10
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <reframework/API.hpp>