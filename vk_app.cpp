//
// vk_app.cpp
//

#include "vk_app.h"

void vk_app::Begin()
{
	// Create VK Instance & Physical Devices & Query Infos.
	{
		// Enum instance layer props.
		{
			uint32_t layerPropCount = 0;
			_vk_try(vkEnumerateInstanceLayerProperties(&layerPropCount, nullptr));
			if (layerPropCount > 0)
			{
				m_instanceLayerProps.resize(layerPropCount);
				_vk_try(vkEnumerateInstanceLayerProperties(&layerPropCount, m_instanceLayerProps.data()));
			}
		}

		// Query the instance extensions.
		{
			uint32_t extCount = 0;
			_vk_try(vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr));
			if (extCount > 0)
			{
				m_instanceExtProps.resize(extCount);
				_vk_try(vkEnumerateInstanceExtensionProperties(nullptr, &extCount, m_instanceExtProps.data()));
			}
		}
		
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "VK_Application";
		appInfo.applicationVersion = 1;
		appInfo.apiVersion = VK_MAKE_VERSION(1, 0, 0);

		VkInstanceCreateInfo instanceCreateInfo = {};
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfo.pApplicationInfo = &appInfo;
		instanceCreateInfo.enabledLayerCount = 0;
		instanceCreateInfo.ppEnabledLayerNames = nullptr;
		instanceCreateInfo.enabledExtensionCount = 0;
		instanceCreateInfo.ppEnabledExtensionNames = nullptr;

		_vk_try(vkCreateInstance(&instanceCreateInfo, &(VkAllocationCallbacks)m_allocator, &m_instance));
	
		// Enum physical devices.
		uint32_t physicalDeviceCount = 0;
		_vk_try(vkEnumeratePhysicalDevices(m_instance, &physicalDeviceCount, nullptr));
		_exit_log(physicalDeviceCount == 0, "Can't find any physical devices on the host!");

		m_physicalDevices.resize(physicalDeviceCount);
		_vk_try(vkEnumeratePhysicalDevices(m_instance, &physicalDeviceCount, m_physicalDevices.data()));

		// Resize std::vector. 
		m_PDExtProps.resize(physicalDeviceCount);
		m_PDLayerProps.resize(physicalDeviceCount);
		m_physicalDevicesProps.resize(physicalDeviceCount);
		m_physicalDevicesFeatures.resize(physicalDeviceCount);
		m_physicalDevicesMemProps.resize(physicalDeviceCount);
		m_queueFamilyProps.resize(physicalDeviceCount);

		for (uint32_t i = 0; i < physicalDeviceCount; ++i)
		{
			// Enum physical device layer props.
			{
				uint32_t layerPropCount = 0;
				_vk_try(vkEnumerateDeviceLayerProperties(m_physicalDevices[i], &layerPropCount, nullptr));
				if (layerPropCount > 0)
				{
					m_PDLayerProps[i].resize(layerPropCount);
					_vk_try(vkEnumerateDeviceLayerProperties(m_physicalDevices[i], &layerPropCount, m_PDLayerProps[i].data()));
				}
			}

			// Query the physical device extensions.
			{
				uint32_t extCount = 0;
				_vk_try(vkEnumerateDeviceExtensionProperties(m_physicalDevices[i], nullptr, &extCount, nullptr));
				if (extCount > 0)
				{
					m_PDExtProps[i].resize(extCount);
					_vk_try(vkEnumerateDeviceExtensionProperties(m_physicalDevices[i], nullptr, &extCount, m_PDExtProps[i].data()));
				}
			}

			// Get physical device's Props & Features & Queue Family Props.
			vkGetPhysicalDeviceProperties(m_physicalDevices[i], &m_physicalDevicesProps[i]);
			vkGetPhysicalDeviceFeatures(m_physicalDevices[i], &m_physicalDevicesFeatures[i]);
			vkGetPhysicalDeviceMemoryProperties(m_physicalDevices[i], &m_physicalDevicesMemProps[i]);

			uint32_t queueFamilyPropCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevices[i], &queueFamilyPropCount, nullptr);
			if (queueFamilyPropCount > 0)
			{
				m_queueFamilyProps[i].resize(queueFamilyPropCount);
				vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevices[i], &queueFamilyPropCount, m_queueFamilyProps[i].data());
			}		

			// Find DISCRETE_GPU first.
			if (m_defaultPDIndex != -1)
				continue;
			switch (m_physicalDevicesProps[i].deviceType)
			{			
			case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
				m_defaultPDIndex = i;
				break;
			case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
				m_defaultPDIndex = i;
				break;
			case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
				m_defaultPDIndex = i;
				break;
			case VK_PHYSICAL_DEVICE_TYPE_CPU:
				m_defaultPDIndex = i;
				break;
			default:
				m_defaultPDIndex = -1;
				break;
			}
		}

		_exit_log(m_defaultPDIndex == -1, "Can't find any valid physical devices on the host!");
	}

	// Create VK Logical 
	{
		VkDeviceQueueCreateInfo deviceQueueCreateInfo = {};
		deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		deviceQueueCreateInfo.queueFamilyIndex = 0;       // First queue family.
		deviceQueueCreateInfo.queueCount = 1;             // Only one queue to use.
		deviceQueueCreateInfo.pQueuePriorities = nullptr; // Default priority. 

		// Set required physical device features.
		m_requiredPDFeatures.multiDrawIndirect = m_physicalDevicesFeatures[m_defaultPDIndex].multiDrawIndirect;
		m_requiredPDFeatures.tessellationShader = VK_TRUE;
		m_requiredPDFeatures.geometryShader = VK_TRUE;

		VkDeviceCreateInfo deviceCreateInfo = {};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.queueCreateInfoCount = 1;
		deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
		deviceCreateInfo.pEnabledFeatures = &m_requiredPDFeatures;
		deviceCreateInfo.enabledLayerCount = 0;
		deviceCreateInfo.ppEnabledLayerNames = nullptr;
		deviceCreateInfo.enabledExtensionCount = 0;
		deviceCreateInfo.ppEnabledExtensionNames = nullptr;

		_vk_try(vkCreateDevice(m_physicalDevices[m_defaultPDIndex], &deviceCreateInfo, nullptr, &m_device));
	}
	
}

void vk_app::End()
{
	vkDestroyDevice(m_device, nullptr);
	vkDestroyInstance(m_instance, nullptr);	
}
