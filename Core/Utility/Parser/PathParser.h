/*********************************************************************
 *  PathParser.h
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 * 
 *  A set of file path parse API.
 *********************************************************************/

#pragma once

#include "Core/TypeDef.h"

class PathParser
{

public:

	/**
	 *  Convert relative path to absolute path.
	 * 
	 *  @param  InPath  relative path.
	 * 
	 *  @return absolute path.
	 */
	static const string Parse(const string& InPath);
};

struct Path
{
	Path() = delete;
	Path(const string& InPath) :
		InternalPath(PathParser::Parse(InPath))
	{}

	Path(const Path& InPath)
	{
		InternalPath = InPath.InternalPath;
	}

	Path(const EInitFlag& InFlag)
	{
		InternalPath = _str_null;
	}

	string ToString() const
	{
		return InternalPath;
	}

	const char* ToCString() const
	{
		return InternalPath.c_str();
	}

private:

	string InternalPath;
};