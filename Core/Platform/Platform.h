/*********************************************************************
 *  Platform.h
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 * 
 *  .
 *********************************************************************/
#pragma once

#define PLATFORM_WINDOW                  1

#define VK_USE_PLATFORM_WIN32_KHR        PLATFORM_WINDOW
#define VK_USE_PLATFORM_DISPLAY_KHR      1

// This code block should write before #include <windows.h>
#ifdef WIN32
#define _platform "Win32"
#else
#define _platform "Win64"
#endif // WIN32

#if PLATFORM_WINDOW

#include <WinSDKVer.h>
#define _WIN32_WINNT 0x0A00
#include <SDKDDKVer.h>

// Use the C++ standard templated min/max
#define NOMINMAX

// Include <mcx.h> if you need this
#define NOMCX

// Include <winsvc.h> if you need this
#define NOSERVICE

// WinHelp is deprecated
#define NOHELP

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <windowsx.h> // GET_X_LPARAM...

#endif // PLATFORM_WINDOW
