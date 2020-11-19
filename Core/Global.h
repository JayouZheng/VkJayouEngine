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

	VkInstance              GetVkInstance();
	void                    SetVkInstance(const VkInstance& InInstance);
	VkDevice                GetVkDevice();
	void                    SetVkDevice(const VkDevice& InDevice);

	void                    CacheModuleInfo(const ModuleInfo& InModuleInfo);
	std::string             GetModulePath();
	std::string             GetModuleName();

	BaseAllocator*          GetAllocator();
	VkAllocationCallbacks*  GetVkAllocator();
	void                    SetAllocator(BaseAllocator* InAllocator);
	void                    SafeFreeAllocator();

	void                    OnExit();

	void                    IncInstanceRef();
	void                    DecInstanceRef();
	bool                    IsInstanceRefZero();
}
