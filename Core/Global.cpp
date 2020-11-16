﻿//
// Global.cpp
//

#include "Global.h"
#include "Core/Base/BaseAllocator.h"

namespace
{
	static VkInstance               g_instance         = VK_NULL_HANDLE;
	static VkDevice                 g_device           = VK_NULL_HANDLE;
	static BaseAllocator*           g_allocator        = nullptr;
	static Global::ModuleInfo       g_moduleInfo       = { "", _str_null };
	static uint32                   g_instanceRefs     = 0u;
}

VkInstance Global::GetVkInstance()
{
	return g_instance;
}

void Global::SetVkInstance(const VkInstance& InInstance)
{
	g_instance = InInstance;
}

VkDevice Global::GetVkDevice()
{
	return g_device;
}

void Global::SetVkDevice(const VkDevice& InDevice)
{
	g_device = InDevice;
}

void Global::CacheModuleInfo(const ModuleInfo& InModuleInfo)
{
	g_moduleInfo = InModuleInfo;
}

std::string Global::GetModulePath()
{
	return g_moduleInfo.Path;
}

std::string Global::GetModuleName()
{
	return g_moduleInfo.Name;
}

BaseAllocator* Global::GetAllocator()
{
	return g_allocator;
}

VkAllocationCallbacks* Global::GetVkAllocator()
{
	return g_allocator != nullptr ? g_allocator->GetVkAllocator() : nullptr;
}

void Global::SetAllocator(BaseAllocator* InAllocator)
{
	g_allocator = InAllocator;
}

void Global::SafeFreeAllocator()
{
	if (g_allocator != nullptr)
	{
		delete g_allocator;
		g_allocator = nullptr;
	}
}

void Global::OnExit()
{
}

void Global::IncInstanceRef()
{
	g_instanceRefs++;
}

void Global::DecInstanceRef()
{
	g_instanceRefs--;
}

bool Global::IsInstanceRefZero()
{
	return g_instanceRefs == 0;
}
