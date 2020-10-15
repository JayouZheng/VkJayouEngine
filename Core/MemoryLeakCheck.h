//
// MemoryLeakCheck.h
// Note: This header should not be included before any other header that has override the specifier such as new or malloc/alloc.
//       And this header should be included first if it was used. Badly.

#pragma once

#include "Platform.h"

#if PLATFORM_WINDOW

// Enable run-time memory check for debug builds.
#if defined(DEBUG) || defined(_DEBUG)

#ifndef DBG_NEW

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )

#define new DBG_NEW

#endif // DBG_NEW

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#endif // DEBUG

#endif // PLATFORM_WINDOW