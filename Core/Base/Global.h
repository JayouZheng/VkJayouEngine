/*********************************************************************
 *  Global.h
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 * 
 *  .
 *********************************************************************/

#pragma once

#include "BaseType.h"
#include "vulkan/vulkan.hpp"

class BaseAllocator;

namespace Global
{
	void                    SafeFreeAllocator();

	void                    SetVkInstance   (const VkInstance& InInstance);
	void                    SetVkDevice     (const VkDevice&   InDevice);
	void                    SetAllocator    (BaseAllocator*    nAllocator);

	VkInstance              GetVkInstance();
	VkDevice                GetVkDevice();
	BaseAllocator*          GetAllocator();
	VkAllocationCallbacks*  GetVkAllocator();
}
