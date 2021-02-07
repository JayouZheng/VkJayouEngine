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
	static string Parse(const string& InPath);
};

//---------------------------------------------------------------------------
// Define a file path helper structure.
//---------------------------------------------------------------------------

struct Path
{
	Path() = delete;
	Path(const string& InPath);
	Path(const Path& InPath);

	/**
	 *  Force init Path object instance.
	 * 
	 *  @param  InFlag  Any enum value in EInitFlag.
	 */
	Path(const EInitFlag& InFlag);

	/**
	 *  Convert Path object instance to string.
	 * 
	 *  @return 
	 */
	string ToString() const;

	/**
	 *  Convert Path object instance to cstring.
	 * 
	 *  @return 
	 */
	const char* ToCString() const;

private:

	string InternalPath;           ///< Internal store the path, no access directly.
};