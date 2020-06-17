//
// app_allocator.cpp
//

#include "app_allocator.h"

#pragma region VKAPI_CALL

void* VKAPI_CALL app_allocator::_Allocation(
	void* pUserData,
	size_t size,
	size_t alignment,
	VkSystemAllocationScope allocationScope)
{
	return static_cast<app_allocator*>(pUserData)->Allocation(size, alignment, allocationScope);
}

void* VKAPI_CALL app_allocator::_Reallocation(
	void* pUserData,
	void* pOriginal,
	size_t size,
	size_t alignment,
	VkSystemAllocationScope allocationScope)
{
	return static_cast<app_allocator*>(pUserData)->Reallocation(pOriginal, size, alignment, allocationScope);
}

void VKAPI_CALL app_allocator::_Free(
	void* pUserData,
	void* pMemory)
{
	return static_cast<app_allocator*>(pUserData)->Free(pMemory);
}

#pragma endregion

void* app_allocator::Allocation(
	size_t size,
	size_t alignment,
	VkSystemAllocationScope allocationScope)
{
	m_allocCount++;
	_cmd_print_line("Allocation Scope: " + std::to_string(allocationScope));
	return _aligned_malloc(size, alignment);
}

void* app_allocator::Reallocation(
	void* pOriginal,
	size_t size,
	size_t alignment,
	VkSystemAllocationScope allocationScope)
{
	m_reallocCount++;
	_cmd_print_line("Reallocation Scope: " + std::to_string(allocationScope));
	return _aligned_realloc(pOriginal, size, alignment);
}

void app_allocator::Free(void* pMemory)
{
	m_freeCount++;
	_cmd_print_line("Free");
	_aligned_free(pMemory);
}
