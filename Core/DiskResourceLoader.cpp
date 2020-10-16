//
// DiskResourceLoader.cpp
//

#include "DiskResourceLoader.h"
#include "Global.h"

const std::string DiskResourceLoader::Load(const std::string& InPath)
{
	Global::CacheLog("[DiskResourceLoader] Load: " + InPath);

	return Global::GetModulePath() + InPath;
}
