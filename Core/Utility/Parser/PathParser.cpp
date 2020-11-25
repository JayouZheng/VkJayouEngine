//
// PathParser.cpp
//

#include "PathParser.h"
#include "Core/Common.h"

const string PathParser::Parse(const string& InPath)
{
	_log_common("Parse: " + InPath, LogSystem::Category::PathParser);

	return Global::GetModulePath() + InPath;
}
