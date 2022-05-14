/*********************************************************************
 *  PathParser.cpp
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 *********************************************************************/

#include "PathParser.h"
#include "Core/Engine/Engine.h"
#include "../Log/LogSystem.h"

string PathParser::Parse(const string& InPath)
{
	_log_common("Parse: " + InPath, LogSystem::Category::PathParser);

	return Engine::Get()->GetModulePath() + InPath;
}

Path::Path(const string& InPath) :
	InternalPath(PathParser::Parse(InPath))
{}

Path::Path(const Path & InPath)
{
	InternalPath = InPath.InternalPath;
}

Path::Path(const EInitFlag& InFlag)
{
	InternalPath = _str_null;
}

string Path::ToString() const
{
	return InternalPath;
}

const char* Path::ToCString() const
{
	return InternalPath.c_str();
}
