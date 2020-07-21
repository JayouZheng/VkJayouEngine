//
// LogicalDevice.cpp
//

#include "LogicalDevice.h"
#include "CommandQueue.h"

LogicalDevice::LogicalDevice(const VkDevice& InDevice)
	: m_device(InDevice)
{
	
}

LogicalDevice& LogicalDevice::operator=(const VkDevice& InDevice)
{
	m_device = InDevice;
	return *this;
}

LogicalDevice::operator VkDevice()
{
	return m_device;
}

LogicalDevice::operator VkDevice*()
{
	return &m_device;
}

bool LogicalDevice::operator==(const VkDevice& InDevice) const
{
	return m_device == InDevice;
}

void LogicalDevice::SetVkDevice(const VkDevice& InDevice)
{
	m_device = InDevice;
}

CommandQueue LogicalDevice::GetQueue(uint32_t InQueueFamilyIndex, uint32_t InQueueIndex /*= 0*/)
{
	VkQueue vkQueue = VK_NULL_HANDLE;
	vkGetDeviceQueue(m_device, InQueueFamilyIndex, InQueueIndex, &vkQueue);
	return vkQueue;
}

void LogicalDevice::GetSwapchainImagesKHR(VkSwapchainKHR InSwapchain, uint32_t* InOutImageCount, VkImage* OutImages)
{
	_vk_try(vkGetSwapchainImagesKHR(m_device, InSwapchain, InOutImageCount, OutImages));
}

uint32_t LogicalDevice::GetSwapchainNextImageKHR(VkSwapchainKHR InSwapchain, uint64_t InTimeout, VkSemaphore InSemaphore, VkFence InFence)
{
	uint32_t nextImageIndex = _index_0;
	_vk_try(vkAcquireNextImageKHR(m_device, InSwapchain, InTimeout, InSemaphore, InFence, &nextImageIndex));
	return nextImageIndex;
}

VkCommandPool* LogicalDevice::CreateCommandPool(
	uint32_t InQueueFamilyIndex, 
	VkCommandPoolCreateFlags InFlags)
{
	VkCommandPoolCreateInfo cmdPoolCreateInfo = {};
	cmdPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdPoolCreateInfo.flags = InFlags;
	cmdPoolCreateInfo.queueFamilyIndex = InQueueFamilyIndex;

	VkCommandPool* pCmdPool = new VkCommandPool;
	_vk_try(vkCreateCommandPool(m_device, &cmdPoolCreateInfo, nullptr, pCmdPool));
	return pCmdPool;
}

VkCommandPool * LogicalDevice::CreateCommandPool(const VkCommandPoolCreateInfo& InCreateInfo)
{
	VkCommandPool* pCmdPool = new VkCommandPool;
	_vk_try(vkCreateCommandPool(m_device, &InCreateInfo, nullptr, pCmdPool));
	return pCmdPool;
}

VkSwapchainKHR* LogicalDevice::CreateSwapchainKHR(const VkSwapchainCreateInfoKHR& InCreateInfo)
{
	VkSwapchainKHR* pSwapchainKHR = new VkSwapchainKHR;
	_vk_try(vkCreateSwapchainKHR(m_device, &InCreateInfo, nullptr, pSwapchainKHR));
	return pSwapchainKHR;
}

VkShaderModule* LogicalDevice::CreateShaderModule(const VkShaderModuleCreateInfo& InCreateInfo)
{
	VkShaderModule* pShaderModule = new VkShaderModule;
	_vk_try(vkCreateShaderModule(m_device, &InCreateInfo, nullptr, pShaderModule));
	return pShaderModule;
}

VkShaderModule* LogicalDevice::CreateShaderModule(size_t InCodeSize, const uint32_t* InCodes)
{
	VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleCreateInfo.codeSize = InCodeSize;
	shaderModuleCreateInfo.pCode = InCodes;

	VkShaderModule* pShaderModule = new VkShaderModule;
	_vk_try(vkCreateShaderModule(m_device, &shaderModuleCreateInfo, nullptr, pShaderModule));
	return pShaderModule;
}

void LogicalDevice::FlushAll()
{
	_vk_try(vkDeviceWaitIdle(m_device));
}
