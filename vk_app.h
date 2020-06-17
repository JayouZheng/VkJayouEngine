//
// vk_app.h
//

#pragma once

#include "vk_util.h"
#include "app_allocator.h"
#include "vk_ptr.h"

class vk_app
{

public:

	vk_app()
	{

	}
	~vk_app()
	{ 
		End();
	}

	void Begin();
	void End();

public:

	app_allocator m_allocator;

private:

	VkInstance m_instance = VK_NULL_HANDLE;
	VkDevice   m_device   = VK_NULL_HANDLE;

	int32_t                                           m_defaultPDIndex = -1;
	VkPhysicalDeviceFeatures                          m_requiredPDFeatures = {};

	// Physical device infos.
	std::vector<VkPhysicalDevice>                     m_physicalDevices;
	std::vector<VkPhysicalDeviceProperties>           m_physicalDevicesProps;
	std::vector<VkPhysicalDeviceFeatures>             m_physicalDevicesFeatures;
	std::vector<VkPhysicalDeviceMemoryProperties>     m_physicalDevicesMemProps;

	// Query physical device supported buffer/image formats.
	// Simple -> vkGetPhysicalDeviceFormatProperties()
	// Detail -> vkGetPhysicalDeviceImageFormatProperties()

	// Queue family props.
	std::vector<std::vector<VkQueueFamilyProperties>> m_queueFamilyProps;

	// Layers & Extensions.
	std::vector<VkLayerProperties>                    m_instanceLayerProps;
	std::vector<std::vector<VkLayerProperties>>       m_PDLayerProps;
	std::vector<VkExtensionProperties>                m_instanceExtProps;
	std::vector<std::vector<VkExtensionProperties>>   m_PDExtProps;
};