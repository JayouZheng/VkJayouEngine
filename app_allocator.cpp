//
// app_allocator.cpp
//

#include "app_allocator.h"

app_allocator::app_allocator()
{

}

app_allocator::~app_allocator()
{
	_cmd_print_line("Alloc: " + std::to_string(m_allocCount) + " Realloc: " + std::to_string(m_reallocCount) + " Free: " + std::to_string(m_freeCount));
}

void* app_allocator::Allocation(
	size_t size,
	size_t alignment,
	VkSystemAllocationScope allocationScope)
{
	m_allocCount++;
	//_cmd_print_line("Allocation Scope: " + std::to_string(allocationScope));
	return _aligned_malloc(size, alignment);
}

void* app_allocator::Reallocation(
	void* pOriginal,
	size_t size,
	size_t alignment,
	VkSystemAllocationScope allocationScope)
{
	m_reallocCount++;
	//_cmd_print_line("Reallocation Scope: " + std::to_string(allocationScope));
	return _aligned_realloc(pOriginal, size, alignment);
}

void app_allocator::Free(void* pMemory)
{
	m_freeCount++;
	//_cmd_print_line("Free");
	_aligned_free(pMemory);
}
