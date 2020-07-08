//
// BaseAllocator.cpp
//

#include "BaseAllocator.h"

BaseAllocator::BaseAllocator()
{

}

BaseAllocator::~BaseAllocator()
{
	
}

#pragma region VKAPI_CALL

void* VKAPI_CALL BaseAllocator::_Allocation(
	void* pUserData,
	size_t size,
	size_t alignment,
	VkSystemAllocationScope allocationScope)
{
	return static_cast<BaseAllocator*>(pUserData)->Allocation(size, alignment, allocationScope);
}

void* VKAPI_CALL BaseAllocator::_Reallocation(
	void* pUserData,
	void* pOriginal,
	size_t size,
	size_t alignment,
	VkSystemAllocationScope allocationScope)
{
	return static_cast<BaseAllocator*>(pUserData)->Reallocation(pOriginal, size, alignment, allocationScope);
}

void VKAPI_CALL BaseAllocator::_Free(
	void* pUserData,
	void* pMemory)
{
	return static_cast<BaseAllocator*>(pUserData)->Free(pMemory);
}

#pragma endregion
