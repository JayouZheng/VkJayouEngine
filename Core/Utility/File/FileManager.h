//
// FileManager.h
//

#pragma once

#include "Core/TypeDef.h"

class FileUtil
{

public:

	static bool Read  (const string& InPath, std::vector<uint8>& OutData);
	static bool Write (const string& InPath, const std::vector<uint8>& InData);
};