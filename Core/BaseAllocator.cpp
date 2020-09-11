//
// BaseAllocator.cpp
//

#include "BaseAllocator.h"

BaseAllocator::BaseAllocator()
{
	m_allocator.pUserData             = (void*)this;
	m_allocator.pfnAllocation         = &BaseAllocator::_Allocation;
	m_allocator.pfnReallocation       = &BaseAllocator::_Reallocation;
	m_allocator.pfnFree               = &BaseAllocator::_Free;
	m_allocator.pfnInternalAllocation = nullptr; //&BaseAllocator::_InternalAllocation;
	m_allocator.pfnInternalFree       = nullptr; //&BaseAllocator::_InternalFree;
}

BaseAllocator::~BaseAllocator()
{
	
}

VkAllocationCallbacks* BaseAllocator::GetVkAllocator()
{
	return &m_allocator;
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
	static_cast<BaseAllocator*>(pUserData)->Free(pMemory);
}

void VKAPI_CALL BaseAllocator::_InternalAllocation(void* pUserData, size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope)
{
	static_cast<BaseAllocator*>(pUserData)->InternalAllocation(size, allocationType, allocationScope);
}

void VKAPI_CALL BaseAllocator::_InternalFree(void* pUserData, size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope)
{
	static_cast<BaseAllocator*>(pUserData)->InternalFree(size, allocationType, allocationScope);
}

#pragma endregion
