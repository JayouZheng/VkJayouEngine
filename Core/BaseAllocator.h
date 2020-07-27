﻿//
// BaseAllocator.h
//

#pragma once

#include "Common.h"

class BaseAllocator
{

public:

	BaseAllocator();
	virtual ~BaseAllocator();

protected:

	VkAllocationCallbacks m_allocator;

public:

	VkAllocationCallbacks* GetVkAllocator();

private:

	// Declare the allocator callbacks as static member functions.
	static void* VKAPI_CALL _Allocation(void* pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope);

	static void* VKAPI_CALL _Reallocation(void* pUserData, void* pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope);

	static void  VKAPI_CALL _Free(void* pUserData, void* pMemory);

private:

	// Now declare the non static member functions that will actually perform the allocations.
	virtual void* Allocation(size_t size, size_t alignment, VkSystemAllocationScope allocationScope) = 0;

	virtual void* Reallocation(void* pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope) = 0;

	virtual void  Free(void* pMemory) = 0;
};