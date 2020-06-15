//
// vk_app.h
//

#pragma once

#include "vk_util.h"

class vk_app
{

public:

	vk_app() {}
	~vk_app() { End(); }

	void Begin();
	void End();

private:

	VkInstance m_instance = nullptr;
	VkDevice   m_device   = nullptr;

	int32_t                                           m_defaultPDIndex = -1;
	VkPhysicalDeviceFeatures                          m_requiredPDFeatures = {};

	std::vector<VkPhysicalDevice>                     m_physicalDevices;
	std::vector<VkPhysicalDeviceProperties>           m_physicalDevicesProps;
	std::vector<VkPhysicalDeviceFeatures>             m_physicalDevicesFeatures;
	std::vector<VkPhysicalDeviceMemoryProperties>     m_physicalDevicesMemProps;
	std::vector<std::vector<VkQueueFamilyProperties>> m_queueFamilyProps;

	// Layers & Extensions.
	std::vector<VkLayerProperties>                    m_instanceLayerProps;
	std::vector<std::vector<VkLayerProperties>>       m_PDLayerProps;
	std::vector<VkExtensionProperties>                m_instanceExtProps;
	std::vector<std::vector<VkExtensionProperties>>   m_PDExtProps;
};