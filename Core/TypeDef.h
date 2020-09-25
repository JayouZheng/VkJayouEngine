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


#define _assert(x) assert(x)

// Size of a static C-style array. Don't use on pointers!
#define _array_size(x) ((int)(sizeof(x) / sizeof(*x)))

#define _is_cstr_equal(str1, str2) !strcmp(str1, str2)

#define _lambda_is_cstr_equal [&](const char* a, const char* b) { return _is_cstr_equal(a, b); }

#define _lambda_is_equal(Type) [&](const Type& a, const Type& b) { return a == b; }

#define _lambda_is_surface_format_equal [&](const VkSurfaceFormatKHR& a, const VkSurfaceFormatKHR& b) { return (a.format == b.format) && (a.colorSpace == b.colorSpace); }

#define _cmd_print_line(str) std::cout << str << std::endl
#define _cmd_print_line_ws(str) std::wcout << str << std::endl


#define _reinterpret_data(dst, source) { auto data = source; std::memcpy(&dst, &data, sizeof dst); }

// None Zero new.
#define _safe_new(type, num) num != 0u ? new type[num] : nullptr;

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
#define _max_path          260

#define _false             0
#define _true              1

#define _str_null          "NULL"
#define _wstr_null         L"NULL"

#define _numeric_max(Type)  std::numeric_limits<Type>::max()


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

using iBool = int32;
using uBool = uint32;

// Function Ptr Table.
typedef void(__cdecl *PFVOID)    (void);
typedef void(__cdecl *PFVOIDVOID)(void);
typedef int (__cdecl *PFINTVOID) (void);


enum EInitFlag
{
	IF_Init,
	IF_Zero
};