//
// app_allocator.h
//

#pragma once

#include "Core/BaseAllocator.h"

class app_allocator : public BaseAllocator
{

public:

	app_allocator();
	~app_allocator();

private:

	// Now declare the non static member functions that will actually perform the allocations.
	virtual void* Allocation(size_t size, size_t alignment, VkSystemAllocationScope allocationScope) override;

	virtual void* Reallocation(void* pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope) override;

	virtual void  Free(void* pMemory) override;

	virtual void  InternalAllocation(size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope) override;

	virtual void  InternalFree(size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope) override;

private:

	uint32 m_allocCount = 0;
	uint32 m_reallocCount = 0;
	uint32 m_freeCount = 0;

	std::vector<void*> m_allocAddress;
	std::vector<uint64> m_freeAddress;
};
