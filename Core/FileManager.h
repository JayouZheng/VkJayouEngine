//
// FileManager.h
//

#pragma once

#include "TypeDef.h"

class FileUtil
{

public:

	static bool Read  (const std::string& InPath, std::vector<uint8>& OutData);
	static bool Write (const std::string& InPath, const std::vector<uint8>& InData);
};