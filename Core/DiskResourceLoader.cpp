//
// DiskResourceLoader.cpp
//

#include "DiskResourceLoader.h"
#include "Global.h"

const std::string DiskResourceLoader::Load(const std::string& InPath)
{
	LogSystem::Log("Load: " + InPath, LogSystem::Category::DiskResourceLoader);

	return Global::GetModulePath() + InPath;
}
