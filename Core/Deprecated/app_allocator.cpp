//
// app_allocator.cpp
// Note: This allocator demo is copied from "Vulkan™ Programming Guide, The Official Guide to Learning Vulkan", 
//       And I have found that the use of this allocator will result in memory leak. 

#include "app_allocator.h"

#if ENABLE_CUSTOM_ALLOCATOR

#include "Core/Global.h"

#include "Core/Utility/MemoryLeakCheck.h"

namespace
{
	size_t GetAlignedSize(size_t InSize, size_t InAlignment)
	{
		return 0;
	}
}

app_allocator::app_allocator()
{

}

app_allocator::~app_allocator()
{
	Global::CacheLog("Alloc: " + std::to_string(m_allocCount) + " Realloc: " + std::to_string(m_reallocCount) + " Free: " + std::to_string(m_freeCount));
}

void* app_allocator::Allocation(
	size_t size,
	size_t alignment,
	VkSystemAllocationScope allocationScope)
{
	m_allocCount++;
	return _aligned_malloc(size, alignment);
}

void* app_allocator::Reallocation(
	void* pOriginal,
	size_t size,
	size_t alignment,
	VkSystemAllocationScope allocationScope)
{
	m_reallocCount++;
	return _aligned_realloc(pOriginal, size, alignment);
}

void app_allocator::Free(void* pMemory)
{
	m_freeCount++;
	_aligned_free(pMemory);
}

void app_allocator::InternalAllocation(
	size_t size, 
	VkInternalAllocationType allocationType, 
	VkSystemAllocationScope allocationScope)
{
	Global::CacheLog("size: " + std::to_string(size) + ", alloc type: " + std::to_string(allocationType) + ", alloc scope: " + std::to_string(allocationScope));
}

void app_allocator::InternalFree(
	size_t size, 
	VkInternalAllocationType allocationType, 
	VkSystemAllocationScope allocationScope)
{
	Global::CacheLog("size: " + std::to_string(size) + ", alloc type: " + std::to_string(allocationType) + ", alloc scope: " + std::to_string(allocationScope));
}

#endif
