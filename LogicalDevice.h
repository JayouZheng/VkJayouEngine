//
// LogicalDevice.h
//

#pragma once

#include "vk_util.h"

class CommandQueue;

class LogicalDevice
{

protected:

	VkDevice m_device = VK_NULL_HANDLE;

public:

	LogicalDevice() = delete;
	LogicalDevice(const VkDevice& InDevice);
	LogicalDevice& operator=(const VkDevice& InDevice);

	virtual ~LogicalDevice() {}

public:

	operator VkDevice();
	operator VkDevice*();

	bool operator==(const VkDevice& InDevice) const;

public:

	void SetVkDevice(const VkDevice& InDevice);

public:

	CommandQueue GetQueue                 (uint32_t InQueueFamilyIndex, uint32_t InQueueIndex = _index_0);
	void         GetSwapchainImagesKHR    (VkSwapchainKHR InSwapchain, uint32_t* InOutImageCount, VkImage* OutImages);
	uint32_t     GetSwapchainNextImageKHR (VkSwapchainKHR InSwapchain, uint64_t InTimeout, VkSemaphore InSemaphore, VkFence InFence);

	VkCommandPool * CreateCommandPool(
		uint32_t InQueueFamilyIndex, 
		VkCommandPoolCreateFlags InFlags = 
		VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | 
		VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

	VkCommandPool * CreateCommandPool  (const VkCommandPoolCreateInfo & InCreateInfo);
	VkSwapchainKHR* CreateSwapchainKHR (const VkSwapchainCreateInfoKHR& InCreateInfo);
	VkShaderModule* CreateShaderModule (const VkShaderModuleCreateInfo& InCreateInfo);

	VkShaderModule* CreateShaderModule (size_t InCodeSize, const uint32_t* InCodes);

	void FlushAll();

};