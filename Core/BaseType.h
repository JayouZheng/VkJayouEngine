//
// BaseType.h
// Note: Differ from Platform...

#pragma once

#include <cstdint>

using int8 = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;

using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

using u16char = char16_t;
using u32char = char32_t;

using byte = uint8;

using iBool = int32;
using uBool = uint32;