//
// TypeDef.h
//

#pragma once

#include <cassert>
#include <cstring>
#include <cstdio>
#include <cstdlib>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <unordered_map>
#include <memory>
#include <tuple>
#include <type_traits>

// Core Level Using, Json Parser...
#include "json/json.h"

#define _assert(x) assert(x)

// Size of a static C-style array. Don't use on pointers!
#define _array_size(x)          ((int)(sizeof(x) / sizeof(*x)))

#define _is_cstr_equal(str1, str2) !strcmp(str1, str2)

#define _lambda_is_cstr_equal [&](const char* a, const char* b) { return _is_cstr_equal(a, b); }

#define _lambda_is_equal(Type) [&](const Type& a, const Type& b) { return a == b; }

#define _lambda_is_surface_format_equal [&](const VkSurfaceFormatKHR& a, const VkSurfaceFormatKHR& b) { return (a.format == b.format) && (a.colorSpace == b.colorSpace); }

#define _cmd_print_line(str) std::cout << str << std::endl
#define _cmd_print_line_ws(str) std::wcout << str << std::endl

#define _bexit_log(b, log) if(b) { std::cout << log << std::endl; exit(1); }

#define _return_log(log) { Global::CacheLog(log); return; }
#define _breturn_log(b, log) if (b) { Global::CacheLog(log); return; }
#define _returnx_log(ret, log) { Global::CacheLog(log); return ret; }

#define _bcontinue_log(b, log) if (b) { Global::CacheLog(log); continue; }
#define _bbreak_log(b, log) if (b) { Global::CacheLog(log); break; }


#define _is_guaranteed_min(x, min_val, y) { if (Global::IsVkGuaranteedMinimum<uint32>(x, min_val)) x = std::min(x, y); }

// Json Cpp Util Defines.
#define _jverify_return_log(json_key, log) if (json_key == Json::nullValue) { Global::CacheLog(log); return; }
#define _jget_int(json_key) (json_key == Json::nullValue) ? 0 : json_key.asInt()
#define _jget_uint(json_key) (json_key == Json::nullValue) ? 0u : json_key.asUInt()
#define _jget_float(json_key) (json_key == Json::nullValue) ? 0.0f : json_key.asFloat()

#define _jget_cstring_default(json_key, default) json_key.isString() ? ((json_key == Json::nullValue) ? default : (*json_key.asCString() == 0 ? default : json_key.asCString())) : default
#define _jget_cstring(json_key) _jget_cstring_default(json_key, "null")
#define _jget_string(json_key) std::string(_jget_cstring_default(json_key, "null"))
#define _jget_string_default(json_key, default) std::string(_jget_cstring_default(json_key, default))

#define _jis_auto(json_key) _jget_string(json_key) == "auto"


#define _reinterpret_data(dst, source) { auto data = source; std::memcpy(&dst, &data, sizeof dst); }

// Safe delete.
#define _safe_delete(ptr) if (ptr != nullptr) { delete ptr; ptr = nullptr; }
#define _safe_delete_array(ptr) if (ptr != nullptr) { delete[] ptr; ptr = nullptr; }


#define _name_of(x)   #x
#define _wname_of(x) L#x

#define _str_name_of(x) std::string(_name_of(x))

#define _nullptr (void*)0


#define _index_0           0
#define _count_0           0
#define _count_1           1
#define _offset_0          0
#define _offset_start      0
#define _flag_none         0
#define _max_path          256


#include "vulkan/vk_platform.h"

using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;

using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

using byte = uint8;

// Function Ptr Table.
typedef void(__cdecl *PFVOID)(void);
typedef void(__cdecl *PFVOIDVOID)(void);
typedef int(__cdecl *PFINTVOID)(void);


enum EInitFlag
{
	IF_Init,
	IF_Zero
};