﻿//
// JsonParser.cpp
//

#include "JsonParser.h"
#include "Core/Utility/LogSystem.h"

bool JsonParser::Parse(const string& InPath, Json::Value& OutRoot)
{
	std::ifstream ifs(InPath, std::ifstream::in);

	if (ifs.is_open())
	{
		Json::CharReaderBuilder builder;
		JSONCPP_STRING errs;

		if (!parseFromStream(builder, ifs, &OutRoot, &errs))
		{
			_log_error(errs, LogSystem::Category::JsonParser);
			return false;
		}

		return true;
	}
	else
	{
		_log_error("Can't open Json file \"" + InPath + "\"", LogSystem::Category::JsonParser);
		return false;
	}
}
