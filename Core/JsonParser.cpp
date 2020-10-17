//
// JsonParser.cpp
//

#include "JsonParser.h"
#include "LogSystem.h"

void JsonParser::Log(const std::string& InLog)
{
	LogSystem::Log(InLog, LogSystem::Category::JsonParser);
}
