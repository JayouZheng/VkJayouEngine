//
// Global.cpp
//

#include "Global.h"
#include "BaseAllocator.h"

namespace
{
	static VkInstance g_instance = VK_NULL_HANDLE;
	static LogicalDevice g_device = VK_NULL_HANDLE;

	static BaseAllocator* g_allocator = nullptr;

	static uint32 g_count = _count_0;
	static bool g_bDestroyManually = false;
}

VkInstance Global::GetVkInstance()
{
	_exit_log(g_instance == VK_NULL_HANDLE, "return [g_instance] is null!");
	return g_instance;
}

void Global::SetVkInstance(const VkInstance& InInstance)
{
	g_instance = InInstance;
}


LogicalDevice Global::GetLogicalDevice()
{
	_exit_log(g_device == VK_NULL_HANDLE, "return [g_device] is null!");
	return g_device;
}

void Global::SetLogicalDevice(const LogicalDevice& InLogicalDevice)
{
	g_device = InLogicalDevice;
}

VkDevice Global::GetVkDevice()
{
	_exit_log(g_device == VK_NULL_HANDLE, "return [g_device] is null!");
	return (VkDevice)g_device;
}

void Global::SetVkDevice(const VkDevice& InDevice)
{
	g_device.SetVkDevice(InDevice);
}

void Global::Decrease()
{
	g_count--;
}

void Global::Advance()
{
	g_count++;
}

bool Global::IsZero()
{
	return g_count == 0;
}

void Global::ApplicationDestroyManually(bool InFlag)
{
	g_bDestroyManually = InFlag;
}

bool Global::IsDestroyManually()
{
	return g_bDestroyManually;
}

BaseAllocator* Global::GetAllocator()
{
	return g_allocator;
}

VkAllocationCallbacks* Global::GetVkAllocator()
{
	return (g_allocator != nullptr) ? (VkAllocationCallbacks*)g_allocator : nullptr;
}

void Global::SetAllocator(BaseAllocator* InAllocator)
{
	g_allocator = InAllocator;
}

void Global::SafeFreeAllocator()
{
	if (g_allocator != nullptr)
	{
		delete g_allocator;
		g_allocator = nullptr;
	}
}
