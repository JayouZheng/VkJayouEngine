//
// app_allocator.h
// Note: This allocator demo is copied from "Vulkan™ Programming Guide, The Official Guide to Learning Vulkan", 
//       And I have found that the use of this allocator will result in memory leak. 

#pragma once

#define ENABLE_CUSTOM_ALLOCATOR  0

#if ENABLE_CUSTOM_ALLOCATOR

#include "Core/BaseAllocator.h"

class app_allocator : public BaseAllocator
{

public:

	app_allocator();
	~app_allocator();

private:

	virtual void* Allocation(size_t size, size_t alignment, VkSystemAllocationScope allocationScope) override;

	virtual void* Reallocation(void* pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope) override;

	virtual void  Free(void* pMemory) override;

	virtual void  InternalAllocation(size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope) override;

	virtual void  InternalFree(size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope) override;

private:

	uint32 m_allocCount   = 0;
	uint32 m_reallocCount = 0;
	uint32 m_freeCount    = 0;
};

#endif
