﻿//
// app_allocator.h
//

#pragma once

#include "BaseAllocator.h"

class app_allocator : public BaseAllocator
{

public:

	app_allocator();
	~app_allocator();

private:

	// Now declare the non static member functions that will actually perform the allocations.
	virtual void* Allocation(size_t size, size_t alignment, VkSystemAllocationScope allocationScope) override;

	virtual void* Reallocation(void* pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope) override;

	virtual void Free(void* pMemory) override;

private:

	uint32_t m_allocCount = 0;
	uint32_t m_reallocCount = 0;
	uint32_t m_freeCount = 0;
};