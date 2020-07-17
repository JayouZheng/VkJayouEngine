//
// BaseLayer.cpp
//

#include "BaseLayer.h"
#include "Global.h"
#include "Window.h"

BaseLayer::BaseLayer()
{

}

BaseLayer::BaseLayer(BaseAllocator* InAllocator)
	: m_allocator(InAllocator)
{

}

BaseLayer::~BaseLayer()
{
	Free();
}

void BaseLayer::Init()
{
	// Create VK Instance & Physical Devices & Query Infos.
	{
		// Enum instance layer props.
		{
			uint32_t layerPropCount = _count_0;
			_vk_try(vkEnumerateInstanceLayerProperties(&layerPropCount, nullptr));
			if (layerPropCount > 0)
			{
				m_instanceLayerProps.resize(layerPropCount);
				_vk_try(vkEnumerateInstanceLayerProperties(&layerPropCount, m_instanceLayerProps.data()));
			}
		}

		// Query the instance extensions.
		{
			uint32_t extCount = _count_0;
			_vk_try(vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr));
			if (extCount > 0)
			{
				m_instanceExtProps.resize(extCount);
				_vk_try(vkEnumerateInstanceExtensionProperties(nullptr, &extCount, m_instanceExtProps.data()));
			}
		}

		// Check Extensions Support.		
		{
			int NumEnableExts = _array_size(BaseLayerConfig::EnableExtensions);
			for (auto& prop : m_instanceExtProps)
			{				
				for (int i = 0; i < NumEnableExts; ++i)
				{
					if (_is_cstr_equal(prop.extensionName, BaseLayerConfig::EnableExtensions[i]))
					{
						m_supportEnableExts.push_back(BaseLayerConfig::EnableExtensions[i]);
					}
				}
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
		instanceCreateInfo.enabledLayerCount = _count_0;
		instanceCreateInfo.ppEnabledLayerNames = nullptr;
		instanceCreateInfo.enabledExtensionCount = (uint32_t)m_supportEnableExts.size();
		instanceCreateInfo.ppEnabledExtensionNames = m_supportEnableExts.data();

		VkInstance vkInstance = VK_NULL_HANDLE;
		_vk_try(vkCreateInstance(&instanceCreateInfo, &(VkAllocationCallbacks)*m_allocator, &vkInstance));
		Global::SetVkInstance(vkInstance);

		// Enum physical devices.
		uint32_t physicalDeviceCount = _count_0;
		_vk_try(vkEnumeratePhysicalDevices(Global::GetVkInstance(), &physicalDeviceCount, nullptr));
		_exit_log(physicalDeviceCount == 0, "Can't find any physical devices on the host!");

		m_physicalDevices.resize(physicalDeviceCount);
		_vk_try(vkEnumeratePhysicalDevices(Global::GetVkInstance(), &physicalDeviceCount, m_physicalDevices.data()));

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
				uint32_t layerPropCount = _count_0;
				_vk_try(vkEnumerateDeviceLayerProperties(m_physicalDevices[i], &layerPropCount, nullptr));
				if (layerPropCount > 0)
				{
					m_PDLayerProps[i].resize(layerPropCount);
					_vk_try(vkEnumerateDeviceLayerProperties(m_physicalDevices[i], &layerPropCount, m_PDLayerProps[i].data()));
				}
			}

			// Query the physical device extensions.
			{
				uint32_t extCount = _count_0;
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

			uint32_t queueFamilyPropCount = _count_0;
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

	// Create VK Logical Devices & Get Queue & Create Command Pool.
	{
		// Find Graphic Queue Family.
		int graphicQueueFamilyIndex = 0;
		for (auto& prop : m_queueFamilyProps[m_defaultPDIndex])
		{
			if (prop.queueFlags && VK_QUEUE_GRAPHICS_BIT)
				m_graphicQueueFamilyIndex = graphicQueueFamilyIndex;
			else ++graphicQueueFamilyIndex;			
		}

		_exit_log(m_graphicQueueFamilyIndex == -1, "Can't find any valid Graphic Queue Family!");
		
		VkDeviceQueueCreateInfo deviceQueueCreateInfo = {};
		deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		deviceQueueCreateInfo.queueFamilyIndex = m_graphicQueueFamilyIndex;       // Graphic Queue Family.
		deviceQueueCreateInfo.queueCount = _count_1;                              // Only one queue to use.
		deviceQueueCreateInfo.pQueuePriorities = nullptr;                         // Default priority. 

		// Set required physical device features.
		m_requiredPDFeatures.multiDrawIndirect = m_physicalDevicesFeatures[m_defaultPDIndex].multiDrawIndirect;
		m_requiredPDFeatures.tessellationShader = VK_TRUE;
		m_requiredPDFeatures.geometryShader = VK_TRUE;

		VkDeviceCreateInfo deviceCreateInfo = {};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.queueCreateInfoCount = _count_1;
		deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
		deviceCreateInfo.pEnabledFeatures = &m_requiredPDFeatures;
		deviceCreateInfo.enabledLayerCount = _count_0;
		deviceCreateInfo.ppEnabledLayerNames = nullptr;
		deviceCreateInfo.enabledExtensionCount = _count_0;
		deviceCreateInfo.ppEnabledExtensionNames = nullptr;

		VkDevice vkDevice = VK_NULL_HANDLE;
		_vk_try(vkCreateDevice(m_physicalDevices[m_defaultPDIndex], &deviceCreateInfo, nullptr, &vkDevice));
		Global::SetVkDevice(vkDevice);

		vkGetDeviceQueue(Global::GetVkDevice(), m_graphicQueueFamilyIndex, _index_0, &m_queue);

		VkCommandPoolCreateInfo cmdPoolCreateInfo = {};
		cmdPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		cmdPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT & VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		cmdPoolCreateInfo.queueFamilyIndex = m_graphicQueueFamilyIndex;

		_vk_try(vkCreateCommandPool(Global::GetVkDevice(), &cmdPoolCreateInfo, nullptr, m_pCmdPool));

#if VK_USE_PLATFORM_WIN32_KHR

		VkBool32 bIsDefaultQueueSupportPresentation = vkGetPhysicalDeviceWin32PresentationSupportKHR(m_physicalDevices[m_defaultPDIndex], m_graphicQueueFamilyIndex);
		_exit_log(bIsDefaultQueueSupportPresentation == VK_FALSE, "The Default Queue Do Not Support Presentation!");
		
		m_window = new Window;
		VkWin32SurfaceCreateInfoKHR win32SurfaceCreateInfo = {};
		win32SurfaceCreateInfo.sType = VK_STRUCTURE_TYPE_DISPLAY_SURFACE_CREATE_INFO_KHR;
		win32SurfaceCreateInfo.hinstance = (HINSTANCE)m_window->GetHinstance();
		win32SurfaceCreateInfo.hwnd = (HWND)m_window->GetHwnd();

		_vk_try(vkCreateWin32SurfaceKHR(Global::GetVkInstance(), &win32SurfaceCreateInfo, nullptr, &m_surface));

		m_window->Show();

#endif
	}

}

void BaseLayer::Free()
{
	if (m_window != nullptr)
	{
		delete m_window;
		m_window = nullptr;
	}

#if 0
	if (Global::IsDestroyManually())
	{
		// If app has none vk_ptr, this codes should be activated!
		vkDestroyDevice(Global::GetVkDevice(), nullptr);
		vkDestroyInstance(Global::GetVkInstance(), nullptr);
	}
#endif
}

uint32_t BaseLayer::GetHeapIndexFromMemPropFlags(
	const VkMemoryRequirements& InMemRequirements, 
	VkMemoryPropertyFlags InPreferredFlags, 
	VkMemoryPropertyFlags InRequiredFlags)
{
	uint32_t selectedIndex = ~0u;
	uint32_t memTypeIndex;

	for (memTypeIndex = 0; memTypeIndex < VK_MAX_MEMORY_TYPES; ++memTypeIndex)
	{
		if (InMemRequirements.memoryTypeBits & (1 << memTypeIndex))
		{
			const VkMemoryType& memType = m_physicalDevicesMemProps[m_defaultPDIndex].memoryTypes[memTypeIndex];

			// If it exactly matches my preferred properties, grab it.
			if ((memType.propertyFlags & InPreferredFlags) == InPreferredFlags)
			{
				selectedIndex = memTypeIndex;
				return selectedIndex;
			}
		}
	}

	for (memTypeIndex = 0; memTypeIndex < VK_MAX_MEMORY_TYPES; ++memTypeIndex)
	{
		if (InMemRequirements.memoryTypeBits & (1 << memTypeIndex))
		{
			const VkMemoryType& memType = m_physicalDevicesMemProps[m_defaultPDIndex].memoryTypes[memTypeIndex];

			// If it has any my required properties, it'll do.
			if ((memType.propertyFlags & InRequiredFlags) == InRequiredFlags)
			{
				selectedIndex = memTypeIndex;
				return selectedIndex;
			}
		}
	}

	return selectedIndex;
}

void BaseLayer::CheckFormatSupport(const std::vector<VkFormat>& InCheckFormats, std::vector<VkFormatProperties>& OutFormatProps)
{
	OutFormatProps.clear();
	for (auto& format : InCheckFormats)
	{
		VkFormatProperties formatProps = {};
		vkGetPhysicalDeviceFormatProperties(m_physicalDevices[m_defaultPDIndex], format, &formatProps);
		OutFormatProps.push_back(formatProps);
	}
}

void BaseLayer::CheckImageFormatSupport(const std::vector<SCheckImage>& InCheckImages, std::vector<VkImageFormatProperties>& OutImageFormatProps)
{
	OutImageFormatProps.clear();
	for (auto& image : InCheckImages)
	{
		VkImageFormatProperties imageFormatProps = {};
		_vk_try(vkGetPhysicalDeviceImageFormatProperties(m_physicalDevices[m_defaultPDIndex], 
			image.format, image.type, image.tiling, image.usage, image.flags, &imageFormatProps));
		OutImageFormatProps.push_back(imageFormatProps);
	}
}
