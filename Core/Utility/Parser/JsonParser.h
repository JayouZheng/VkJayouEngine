//
// JsonParser.h
//

#pragma once

#include "json/json.h"
#include "Core/TypeDef.h"

class JsonParser
{

public:

	static bool Parse(const std::string& InPath, Json::Value& OutRoot);

	static inline int32 GetInt32(const Json::Value& InValue)
	{
		return (InValue != Json::nullValue) ? InValue.asInt() : 0;
	}

	static inline uint32 GetUInt32(const Json::Value& InValue)
	{
		return (InValue != Json::nullValue) ? InValue.asUInt() : 0u;
	}

	static inline float GetFloat(const Json::Value& InValue)
	{
		return (InValue != Json::nullValue) ? InValue.asFloat() : .0f;
	}

	static inline std::string GetString(const Json::Value& InValue, const std::string& InDefault = _str_null)
	{
		return (InValue != Json::nullValue) ? (InValue.isString() ? InValue.asString() : InDefault) : InDefault;
	}
};
