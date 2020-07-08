//
// BaseAllocator.h
//

#pragma once

#include "vk_util.h"

class BaseAllocator
{

public:

	// Operator that allows an instance of this class to be used as a
	// VkAllocationCallbacks structure
	inline operator VkAllocationCallbacks() const
	{
		VkAllocationCallbacks result;
		result.pUserData = (void*)this;
		result.pfnAllocation = &BaseAllocator::_Allocation;
		result.pfnReallocation = &BaseAllocator::_Reallocation;
		result.pfnFree = &BaseAllocator::_Free;
		result.pfnInternalAllocation = nullptr;
		result.pfnInternalFree = nullptr;
		return result;
	}

	BaseAllocator();
	virtual ~BaseAllocator();

private:

	// Declare the allocator callbacks as static member functions.
	static void* VKAPI_CALL _Allocation(void* pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope);

	static void* VKAPI_CALL _Reallocation(void* pUserData, void* pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope);

	static void  VKAPI_CALL _Free(void* pUserData, void* pMemory);

private:

	// Now declare the non static member functions that will actually perform the allocations.
	virtual void* Allocation(size_t size, size_t alignment, VkSystemAllocationScope allocationScope) = 0;

	virtual void* Reallocation(void* pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope) = 0;

	virtual void Free(void* pMemory) = 0;
};