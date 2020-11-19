//
// PathParser.cpp
//

#include "PathParser.h"
#include "Core/Common.h"

const std::string PathParser::Parse(const std::string& InPath)
{
	LogSystem::Log("Parse: " + InPath, LogSystem::Category::PathParser);

	return Global::GetModulePath() + InPath;
}
