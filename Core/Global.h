//
// Global.h
//

#pragma once

#include <string>
#include "vulkan/vulkan.hpp"

class BaseAllocator;

namespace Global
{
	struct ModuleInfo
	{
		std::string Path;
		std::string Name;
	};

	void                    SafeFreeAllocator();
	void                    IncInstanceRef();
	void                    DecInstanceRef();
	bool                    IsInstanceRefZero();

	void                    SetVkInstance   (const VkInstance& InInstance);
	void                    SetVkDevice     (const VkDevice&   InDevice);
	void                    SetAllocator    (BaseAllocator*    nAllocator);
	void                    CacheModuleInfo (const ModuleInfo& InModuleInfo);

	VkInstance              GetVkInstance();
	VkDevice                GetVkDevice();
	std::string             GetModulePath();
	std::string             GetModuleName();
	BaseAllocator*          GetAllocator();
	VkAllocationCallbacks*  GetVkAllocator();
}
