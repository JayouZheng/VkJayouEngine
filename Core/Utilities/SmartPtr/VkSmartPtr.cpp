/*********************************************************************
 *  VkSmartPtr.cpp
 *  Copyright (C) 2021 Jayou. All Rights Reserved.
 * 
 *  .
 *********************************************************************/

#include "VkSmartPtr.h"
#include "Core/Base/BaseAllocator.h"

namespace
{
	VkInstance        g_instance     = VK_NULL_HANDLE;
	VkDevice          g_device       = VK_NULL_HANDLE;
	BaseAllocator*    g_allocator    = nullptr;
	int32             g_instanceRefs = 0;
}

void VkSmartPtr_Private::IncInstanceRef()
{
	g_instanceRefs++;
}

void VkSmartPtr_Private::DecInstanceRef()
{
	g_instanceRefs--;
}

bool VkSmartPtr_Private::IsInstanceRefZero()
{
	return g_instanceRefs == 0;
}

void VkSmartPtr_Private::SafeFreeAllocator()
{
	if (g_allocator != nullptr)
	{
		delete g_allocator;
		g_allocator = nullptr;
	}
}

void VkSmartPtr_Private::SetVkInstance(const VkInstance& InInstance)
{
	g_instance = InInstance;
}

void VkSmartPtr_Private::SetVkDevice(const VkDevice& InDevice)
{
	g_device = InDevice;
}

void VkSmartPtr_Private::SetBaseAllocator(BaseAllocator* InAllocator)
{
	g_allocator = InAllocator;
}

VkInstance VkSmartPtr_Private::GetVkInstance()
{
	return g_instance;
}

VkDevice VkSmartPtr_Private::GetVkDevice()
{
	return g_device;
}

BaseAllocator* VkSmartPtr_Private::GetBaseAllocator()
{
	return g_allocator;
}

VkAllocationCallbacks* VkSmartPtr_Private::GetVkAllocator()
{
	return g_allocator != nullptr ? g_allocator->GetVkAllocator() : nullptr;
}
