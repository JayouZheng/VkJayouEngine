//
// Global.h
//

#pragma once

#include "Core/Base/BaseType.h"
#include "vulkan/vulkan.hpp"

class BaseAllocator;

namespace Global
{
	struct ModuleInfo
	{
		string Path;
		string Name;
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
	string             GetModulePath();
	string             GetModuleName();
	BaseAllocator*          GetAllocator();
	VkAllocationCallbacks*  GetVkAllocator();
}
