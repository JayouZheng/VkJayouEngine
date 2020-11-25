/*********************************************************************
 *  FileManager.h
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 * 
 *  A set of file utility API.
 *********************************************************************/

#pragma once

#include "Core/Common.h"

class FileUtil
{

public:

	/**
	 *  Open and read a file with specific path.
	 * 
	 *  @param  InPath   the file path to open.
	 *  @param  OutData  an array of binary data the was read out.
	 * 
	 *  @return true if success, otherwise false.
	 */
	static bool Read(const Path& InPath, std::vector<uint8>& OutData);

	/**
	 *  Open and write a file with specific path.
	 * 
	 *  @param  InPath  the file path to open.
	 *  @param  InData  an array of binary data ot write.
	 * 
	 *  @return true if success, otherwise false.
	 */
	static bool Write (const Path& InPath, const std::vector<uint8>& InData);
};