/*********************************************************************
 *  JsonParser.h
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 * 
 *  A set of josn parse API.
 *********************************************************************/

#pragma once

#ifdef new
#undef new
#include "json/json.h"
#include "Core/Utility/MemoryLeakCheck.h"
#else
#include "json/json.h"
#endif // new

#include "Core/TypeDef.h"

class JsonParser
{

public:

	/**
	 *  Open and parse a json file with specific path.
	 * 
	 *  @param  InPath   the file path to open.
	 *  @param  OutRoot  output json root object.
	 * 
	 *  @return true if success, otherwise false.
	 */
	static bool Parse(const string& InPath, Json::Value& OutRoot);

	/**
	 *  Resolve data of type int32 from json object with caution.
	 * 
	 *  @param  InValue  json object to process.
	 * 
	 *  @return data of type int32.
	 */
	static inline int32 GetInt32(const Json::Value& InValue)
	{
		return (InValue != Json::nullValue) ? InValue.asInt() : 0;
	}

	/**
	 *  Resolve data of type uint32 from json object with caution.
	 * 
	 *  @param  InValue  json object to process.
	 * 
	 *  @return data of type uint32.
	 */
	static inline uint32 GetUInt32(const Json::Value& InValue)
	{
		return (InValue != Json::nullValue) ? InValue.asUInt() : 0u;
	}

	/**
	 *  Resolve data of type float from json object with caution.
	 *
	 *  @param  InValue  json object to process.
	 *
	 *  @return data of type float.
	 */
	static inline float GetFloat(const Json::Value& InValue)
	{
		return (InValue != Json::nullValue) ? InValue.asFloat() : .0f;
	}

	/**
	 *  Resolve data of type string from json object with caution.
	 *
	 *  @param  InValue  json object to process.
	 *
	 *  @return data of type string.
	 */
	static inline string GetString(const Json::Value& InValue, const string& InDefault = _str_null)
	{
		return (InValue != Json::nullValue) ? (InValue.isString() ? InValue.asString() : InDefault) : InDefault;
	}
};
