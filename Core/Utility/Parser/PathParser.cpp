//
// PathParser.cpp
//

#include "PathParser.h"
#include "Core/Common.h"

const std::string PathParser::Parse(const std::string& InPath)
{
	_log_common("Parse: " + InPath, LogSystem::Category::PathParser);

	return Global::GetModulePath() + InPath;
}
