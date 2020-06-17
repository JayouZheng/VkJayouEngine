//
// app_allocator.h
//

#pragma once

#include "vk_util.h"

class app_allocator
{

public:

	// Operator that allows an instance of this class to be used as a
	// VkAllocationCallbacks structure
	inline operator VkAllocationCallbacks() const
	{
		VkAllocationCallbacks result;
		result.pUserData = (void*)this;
		result.pfnAllocation = &app_allocator::_Allocation;
		result.pfnReallocation = &app_allocator::_Reallocation;
		result.pfnFree = &app_allocator::_Free;
		result.pfnInternalAllocation = nullptr;
		result.pfnInternalFree = nullptr;
		return result;
	}

	~app_allocator()
	{
		_cmd_print_line("Alloc: " + std::to_string(m_allocCount) + " Realloc: " + std::to_string(m_reallocCount) + " Free: " + std::to_string(m_freeCount));
	}

private:
	
	// Declare the allocator callbacks as static member functions.
	static void* VKAPI_CALL _Allocation(void* pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope);

	static void* VKAPI_CALL _Reallocation(void* pUserData, void* pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope);

	static void  VKAPI_CALL _Free(void* pUserData, void* pMemory);

	// Now declare the non static member functions that will actually perform the allocations.
	void* Allocation(size_t size, size_t alignment, VkSystemAllocationScope allocationScope);

	void* Reallocation(void* pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope);

	void Free(void* pMemory);

	uint32_t m_allocCount = 0;
	uint32_t m_reallocCount = 0;
	uint32_t m_freeCount = 0;
};
