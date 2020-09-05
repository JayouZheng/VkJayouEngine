//
// Platform.h
//

#pragma once

#define VK_USE_PLATFORM_WIN32_KHR        1
#define VK_USE_PLATFORM_DISPLAY_KHR      1

#if VK_USE_PLATFORM_WIN32_KHR

#include <windows.h>
#undef min
#undef max

#endif