//
// BaseLayer.cpp
//

#include "BaseLayer.h"
#include "Global.h"

#if VK_USE_PLATFORM_WIN32_KHR

// Enable run-time memory check for debug builds.
#if defined(DEBUG) || defined(_DEBUG)

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#endif

#endif

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
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif

	CachedModulePath();

	// Global Variable.
	uint32 numEnableExts   = _array_size(BaseLayerConfig::EnableExtensions);
	uint32 numEnableLayers = _array_size(BaseLayerConfig::EnableLayers);

	// Create VK Instance & Physical Devices & Query Infos.
	{
		// Enum instance layer props.
		{
			uint32 layerPropCount = _count_0;
			_vk_try(vkEnumerateInstanceLayerProperties(&layerPropCount, nullptr));
			if (layerPropCount > 0)
			{
				m_instanceLayerProps.resize(layerPropCount);
				_vk_try(vkEnumerateInstanceLayerProperties(&layerPropCount, m_instanceLayerProps.data()));
			}
		}

		// Query the instance extensions.
		{
			uint32 extCount = _count_0;
			_vk_try(vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr));
			if (extCount > 0)
			{
				m_instanceExtProps.resize(extCount);
				_vk_try(vkEnumerateInstanceExtensionProperties(nullptr, &extCount, m_instanceExtProps.data()));
			}
		}

		// Check Instance Extensions Support.
		{			
			for (auto& prop : m_instanceExtProps)
			{				
				for (uint32 i = 0; i < numEnableExts; ++i)
				{
					if (_is_cstr_equal(prop.extensionName, BaseLayerConfig::EnableExtensions[i]))
					{
						m_supportInsExts.push_back(BaseLayerConfig::EnableExtensions[i]);
					}
				}
			}
		}

		// Check Instance Layers Support.
		{
			for (auto& prop : m_instanceLayerProps)
			{
				for (uint32 i = 0; i < numEnableLayers; ++i)
				{
					if (_is_cstr_equal(prop.layerName, BaseLayerConfig::EnableLayers[i]))
					{
						m_supportInsLayers.push_back(BaseLayerConfig::EnableLayers[i]);
					}
				}
			}
		}

		VkApplicationInfo appInfo  = {};
		appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName   = "VK_Application";
		appInfo.applicationVersion = 1;
		appInfo.apiVersion         = VK_MAKE_VERSION(1, 0, 0);

		VkInstanceCreateInfo instanceCreateInfo = {};
		instanceCreateInfo.sType                = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfo.pApplicationInfo     = &appInfo;
		instanceCreateInfo.enabledLayerCount    = _count_0;
		instanceCreateInfo.ppEnabledLayerNames  = nullptr;

		// Enable Instance Exts.
		{
			uint32 numInsSupportExts = (uint32)m_supportInsExts.size();
			instanceCreateInfo.enabledExtensionCount   = numInsSupportExts;
			instanceCreateInfo.ppEnabledExtensionNames = numInsSupportExts > 0 ? m_supportInsExts.data() : nullptr;
		}

		VkInstance vkInstance = VK_NULL_HANDLE;
		_vk_try(vkCreateInstance(&instanceCreateInfo, m_allocator->GetVkAllocator(), &vkInstance));
		Global::SetVkInstance(vkInstance);

		// Enum physical devices.
		uint32 physicalDeviceCount = _count_0;
		_vk_try(vkEnumeratePhysicalDevices(Global::GetVkInstance(), &physicalDeviceCount, nullptr));
		_bexit_log(physicalDeviceCount == 0, "Can't find any physical devices on the host!");

		m_physicalDevices.resize(physicalDeviceCount);
		_vk_try(vkEnumeratePhysicalDevices(Global::GetVkInstance(), &physicalDeviceCount, m_physicalDevices.data()));

		// Resize std::vector. 
		m_PDExtProps.resize(physicalDeviceCount);
		m_PDLayerProps.resize(physicalDeviceCount);
		m_physicalDevicesProps.resize(physicalDeviceCount);
		m_physicalDevicesFeatures.resize(physicalDeviceCount);
		m_PDVulkan12Features.resize(physicalDeviceCount);
		m_physicalDevicesMemProps.resize(physicalDeviceCount);
		m_queueFamilyProps.resize(physicalDeviceCount);

		for (uint32 i = 0; i < physicalDeviceCount; ++i)
		{
			// Enum physical device layer props.
			{
				uint32 layerPropCount = _count_0;
				_vk_try(vkEnumerateDeviceLayerProperties(m_physicalDevices[i], &layerPropCount, nullptr));
				if (layerPropCount > 0)
				{
					m_PDLayerProps[i].resize(layerPropCount);
					_vk_try(vkEnumerateDeviceLayerProperties(m_physicalDevices[i], &layerPropCount, m_PDLayerProps[i].data()));
				}
			}

			// Query the physical device extensions.
			{
				uint32 extCount = _count_0;
				_vk_try(vkEnumerateDeviceExtensionProperties(m_physicalDevices[i], nullptr, &extCount, nullptr));
				if (extCount > 0)
				{
					m_PDExtProps[i].resize(extCount);
					_vk_try(vkEnumerateDeviceExtensionProperties(m_physicalDevices[i], nullptr, &extCount, m_PDExtProps[i].data()));
				}
			}

			// Get physical device's Props & Features & Queue Family Props.
			for (uint32 j = 0; j < physicalDeviceCount; ++j)
			{
				m_physicalDevicesFeatures[j].sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
				m_PDVulkan12Features[j].sType      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
				m_physicalDevicesFeatures[j].pNext = &m_PDVulkan12Features[j];
			}				

			vkGetPhysicalDeviceProperties(m_physicalDevices[i], &m_physicalDevicesProps[i]);
			vkGetPhysicalDeviceFeatures2(m_physicalDevices[i], &m_physicalDevicesFeatures[i]);
			vkGetPhysicalDeviceMemoryProperties(m_physicalDevices[i], &m_physicalDevicesMemProps[i]);

			uint32 queueFamilyPropCount = _count_0;
			vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevices[i], &queueFamilyPropCount, nullptr);
			if (queueFamilyPropCount > 0)
			{
				m_queueFamilyProps[i].resize(queueFamilyPropCount);
				vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevices[i], &queueFamilyPropCount, m_queueFamilyProps[i].data());
			}

			// Find DISCRETE_GPU first.
			if (m_mainPDIndex != -1)
				continue;
			switch (m_physicalDevicesProps[i].deviceType)
			{
			case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
				m_mainPDIndex = i;
				break;
			case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
				m_mainPDIndex = i;
				break;
			case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
				m_mainPDIndex = i;
				break;
			case VK_PHYSICAL_DEVICE_TYPE_CPU:
				m_mainPDIndex = i;
				break;
			default:
				m_mainPDIndex = -1;
				break;
			}
		}

		_bexit_log(m_mainPDIndex == -1, "Can't find any valid physical devices on the host!");
	}

	// Create VK Logical Devices & Get Queue & Create Command Pool.
	{
		// Find Graphic Queue Family.
		int32 graphicQueueFamilyIndex = 0;
		for (auto& prop : m_queueFamilyProps[m_mainPDIndex])
		{
			if ((prop.queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
				(prop.queueFlags & VK_QUEUE_COMPUTE_BIT))
				m_mainQFIndex = graphicQueueFamilyIndex;
			else ++graphicQueueFamilyIndex;			
		}

		_bexit_log(m_mainQFIndex == -1, "Can't find any Valid Graphic & Compute Queue Family!");
		
		VkDeviceQueueCreateInfo deviceQueueCreateInfo = {};
		deviceQueueCreateInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		deviceQueueCreateInfo.queueFamilyIndex        = m_mainQFIndex;                   // Graphic Queue Family.
		deviceQueueCreateInfo.queueCount              = _count_1;                        // Only one queue to use.
		deviceQueueCreateInfo.pQueuePriorities        = nullptr;                         // Default priority. 

		// Set required physical device features.
		m_requiredPDFeatures.sType                       = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
		m_requiredPDVk12Features.sType                   = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
		m_requiredPDFeatures.features.multiDrawIndirect  = m_physicalDevicesFeatures[m_mainPDIndex].features.multiDrawIndirect;
		m_requiredPDFeatures.features.tessellationShader = m_physicalDevicesFeatures[m_mainPDIndex].features.tessellationShader;
		m_requiredPDFeatures.features.geometryShader     = m_physicalDevicesFeatures[m_mainPDIndex].features.geometryShader;
		m_requiredPDVk12Features.descriptorIndexing      = m_PDVulkan12Features[m_mainPDIndex].descriptorIndexing;
		m_requiredPDFeatures.pNext                       = &m_requiredPDVk12Features;

		// Check PD Extensions Support.
		{
			for (auto& prop : m_PDExtProps[m_mainPDIndex])
			{
				for (uint32 i = 0; i < numEnableExts; ++i)
				{
					if (_is_cstr_equal(prop.extensionName, BaseLayerConfig::EnableExtensions[i]))
					{
						m_supportPDExts.push_back(BaseLayerConfig::EnableExtensions[i]);
					}
				}
			}
		}

		// Check PD Layers Support.
		{
			for (auto& prop : m_PDLayerProps[m_mainPDIndex])
			{
				for (uint32 i = 0; i < numEnableLayers; ++i)
				{
					if (_is_cstr_equal(prop.layerName, BaseLayerConfig::EnableLayers[i]))
					{
						m_supportPDLayers.push_back(BaseLayerConfig::EnableLayers[i]);
					}
				}
			}
		}

		VkDeviceCreateInfo deviceCreateInfo   = {};
		deviceCreateInfo.sType                = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.queueCreateInfoCount = _count_1;
		deviceCreateInfo.pQueueCreateInfos    = &deviceQueueCreateInfo;
		// deviceCreateInfo.pEnabledFeatures  = &m_requiredPDFeatures;
		// VkPhysicalDeviceVulkan12Features,    descriptorIndexing.
		deviceCreateInfo.pNext                = &m_requiredPDFeatures;
		deviceCreateInfo.enabledLayerCount    = _count_0;
		deviceCreateInfo.ppEnabledLayerNames  = nullptr;

		// Enable PD Exts.
		{
			uint32 numPDSupportExts = (uint32)m_supportPDExts.size();
			deviceCreateInfo.enabledExtensionCount   = numPDSupportExts;
			deviceCreateInfo.ppEnabledExtensionNames = numPDSupportExts > 0 ? m_supportPDExts.data() : nullptr;
		}

		_vk_try(vkCreateDevice(m_physicalDevices[m_mainPDIndex], &deviceCreateInfo, m_allocator->GetVkAllocator(), m_device.GetAddressOfVkDevice()));
		m_device.SetBaseLayer(this);
		m_device.SetAllocator(m_allocator);
		Global::SetVkDevice(m_device.GetVkDevice());
		
		m_queue = m_device.GetQueue(m_mainQFIndex);

		m_device.CreateCommandPool(m_mainQFIndex);

#if VK_USE_PLATFORM_WIN32_KHR

		// Create Win32 Surface.
		if (Util::IsVecContain<const char*>(m_supportInsExts, VK_KHR_WIN32_SURFACE_EXTENSION_NAME, _lambda_is_cstr_equal))
		{
			VkBool32 bIsDefaultQueueSupportPresentation = vkGetPhysicalDeviceWin32PresentationSupportKHR(m_physicalDevices[m_mainPDIndex], m_mainQFIndex);
			_bexit_log(bIsDefaultQueueSupportPresentation == VK_FALSE, "The Default Queue Do Not Support Presentation (Win32)!");

			m_window = new Window;
			m_device.SetWindow(m_window);
			VkWin32SurfaceCreateInfoKHR win32SurfaceCreateInfo = {};
			win32SurfaceCreateInfo.sType = VK_STRUCTURE_TYPE_DISPLAY_SURFACE_CREATE_INFO_KHR;
			win32SurfaceCreateInfo.hinstance = (HINSTANCE)m_window->GetHinstance();
			win32SurfaceCreateInfo.hwnd = (HWND)m_window->GetHwnd();

			_vk_try(vkCreateWin32SurfaceKHR(Global::GetVkInstance(), &win32SurfaceCreateInfo, m_allocator->GetVkAllocator(), &m_surface));
		}
		
#endif

		// Create Swapchain. // OnResize Recreate Needed!!!
		if (Util::IsVecContain<const char*>(m_supportPDExts, VK_KHR_SWAPCHAIN_EXTENSION_NAME, _lambda_is_cstr_equal))
		{
			m_swapchainCreateInfo.sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			m_swapchainCreateInfo.surface               = m_surface;
			m_swapchainCreateInfo.imageArrayLayers      = 1;        // Only 1 Layer.
			m_swapchainCreateInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
			m_swapchainCreateInfo.queueFamilyIndexCount = 0;
			m_swapchainCreateInfo.pQueueFamilyIndices   = nullptr;
			m_swapchainCreateInfo.presentMode           = BaseLayerConfig::SwapchainCreateInfo.presentMode;
			m_swapchainCreateInfo.clipped               = BaseLayerConfig::SwapchainCreateInfo.clipped;
			m_swapchainCreateInfo.oldSwapchain          = nullptr;  // First time to Create.

			// Check Support Surface Format For Swapchain.
			{
				uint32 formatCount = _count_0;
				_vk_try(vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevices[m_mainPDIndex], m_surface, &formatCount, nullptr));
				_bexit_log(formatCount == 0, "No Surface Format Support!");

				m_surfaceFormats.resize(formatCount);
				_vk_try(vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevices[m_mainPDIndex], m_surface, &formatCount, m_surfaceFormats.data()));

				m_swapchainCreateInfo.imageFormat = m_surfaceFormats.front().format;
				m_swapchainCreateInfo.imageColorSpace = m_surfaceFormats.front().colorSpace;

				if (Util::IsVecContain<VkSurfaceFormatKHR>(m_surfaceFormats, BaseLayerConfig::SwapchainCreateInfo.surfaceFormat, _lambda_is_surface_format_equal))
				{
					m_swapchainCreateInfo.imageFormat = BaseLayerConfig::SwapchainCreateInfo.surfaceFormat.format;
					m_swapchainCreateInfo.imageColorSpace = BaseLayerConfig::SwapchainCreateInfo.surfaceFormat.colorSpace;
				}
			}

			// Query Surface Capabilities.
			{
				_vk_try(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevices[m_mainPDIndex], m_surface, &m_surfaceCapabilities));

				m_swapchainCreateInfo.imageExtent = m_surfaceCapabilities.currentExtent;

				if ((BaseLayerConfig::SwapchainCreateInfo.frameCount > m_surfaceCapabilities.minImageCount) && 
					(BaseLayerConfig::SwapchainCreateInfo.frameCount < m_surfaceCapabilities.maxImageCount))
					m_swapchainCreateInfo.minImageCount = BaseLayerConfig::SwapchainCreateInfo.frameCount;
				else m_swapchainCreateInfo.minImageCount = m_surfaceCapabilities.minImageCount;

				_bexit_log(!(m_surfaceCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT), "Surface Do Not Support Color Attachment Usage!");
				m_swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT & BaseLayerConfig::SwapchainCreateInfo.imageUsage;

				if (BaseLayerConfig::SwapchainCreateInfo.surfacePreTransform & m_surfaceCapabilities.supportedTransforms)
					m_swapchainCreateInfo.preTransform = BaseLayerConfig::SwapchainCreateInfo.surfacePreTransform;
				else m_swapchainCreateInfo.preTransform = m_surfaceCapabilities.currentTransform;

				if (BaseLayerConfig::SwapchainCreateInfo.compositeAlpha & m_surfaceCapabilities.supportedCompositeAlpha)
					m_swapchainCreateInfo.compositeAlpha = BaseLayerConfig::SwapchainCreateInfo.compositeAlpha;
				else m_swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			}
			
			m_device.CreateSwapchainKHR(m_pSwapchainKHR.MakeInstance(), m_swapchainCreateInfo);

			uint32 swapchainImageCount = _count_0;
			m_device.GetSwapchainImagesKHR(*m_pSwapchainKHR, &swapchainImageCount, nullptr);
			m_swapchainImages.resize(swapchainImageCount);
			m_device.GetSwapchainImagesKHR(*m_pSwapchainKHR, &swapchainImageCount, m_swapchainImages.data());

			// This Code May Be Redundant... Win32 Surface Presentation Support Has Been Queried.
			{
				VkBool32 surfacePresentationSupport = VK_FALSE;
				_vk_try(vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevices[m_mainPDIndex], m_mainQFIndex, m_surface, &surfacePresentationSupport));
				_bexit_log(surfacePresentationSupport == VK_FALSE, "The Default Queue Do Not Support Presentation!");
			}
			
		}
		else _bexit_log(true, "Create Swapchain Failed! Application Terminate!");

		// TODO:
		m_device.CreateGraphicPipelines(nullptr, "Json/graphic_pipeline_info.json");
		
		//m_window->Show();
	}

}

void BaseLayer::CachedModulePath()
{
#if VK_USE_PLATFORM_WIN32_KHR

	std::string modulePath = ""; // It needs to be left empty here.
	std::string moduleName = "null";

	char exePath[_max_path];

	GetModuleFileNameA(NULL, exePath, _max_path);

	modulePath = std::string(exePath);
	std::string::size_type found = modulePath.find_last_of("\\/");
	if (found != std::string::npos)
	{
		moduleName = modulePath.substr(found + 1);
		modulePath = modulePath.substr(0, found + 1);
	}

	// Add path that will remove form modulePath.
	std::vector<std::string> deleteDirs =
	{
		"x64\\Debug\\",
		"x64\\Release\\"
	};

	for (auto& dir : deleteDirs)
	{
		found = std::string::npos;
		found = modulePath.rfind(dir);
		if (found != std::string::npos)
			modulePath.erase(found, dir.size());
	}

	Global::ModuleInfo moduleInfo;
	moduleInfo.Path = modulePath;
	moduleInfo.Name = moduleName;
	Global::CacheModuleInfo(moduleInfo);

#endif
}

void BaseLayer::Free()
{
	Global::PrintLog();
#if 0
	if (Global::IsDestroyManually())
	{
		// If app has none vk_ptr, this codes should be activated!
		vkDestroyDevice(Global::GetVkDevice(), Global::GetVkAllocator());
		vkDestroyInstance(Global::GetVkInstance(), Global::GetVkAllocator());
	}
#endif
}

uint32 BaseLayer::GetHeapIndexFromMemPropFlags(
	const VkMemoryRequirements& InMemRequirements, 
	VkMemoryPropertyFlags InPreferredFlags, 
	VkMemoryPropertyFlags InRequiredFlags)
{
	uint32 selectedIndex = ~0u;
	uint32 memTypeIndex;

	for (memTypeIndex = 0; memTypeIndex < VK_MAX_MEMORY_TYPES; ++memTypeIndex)
	{
		if (InMemRequirements.memoryTypeBits & (1 << memTypeIndex))
		{
			const VkMemoryType& memType = m_physicalDevicesMemProps[m_mainPDIndex].memoryTypes[memTypeIndex];

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
			const VkMemoryType& memType = m_physicalDevicesMemProps[m_mainPDIndex].memoryTypes[memTypeIndex];

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

LogicalDevice BaseLayer::GetLogicalDevice() const
{
	return m_device;
}

VkPhysicalDeviceLimits BaseLayer::GetMainPDLimits() const
{
	return m_physicalDevicesProps[m_mainPDIndex].limits;
}

void BaseLayer::CheckFormatSupport(const std::vector<VkFormat>& InCheckFormats, std::vector<VkFormatProperties>& OutFormatProps)
{
	OutFormatProps.clear();
	for (auto& format : InCheckFormats)
	{
		VkFormatProperties formatProps = {};
		vkGetPhysicalDeviceFormatProperties(m_physicalDevices[m_mainPDIndex], format, &formatProps);
		OutFormatProps.push_back(formatProps);
	}
}

void BaseLayer::CheckImageFormatSupport(const std::vector<SCheckImage>& InCheckImages, std::vector<VkImageFormatProperties>& OutImageFormatProps)
{
	OutImageFormatProps.clear();
	for (auto& image : InCheckImages)
	{
		VkImageFormatProperties imageFormatProps = {};
		_vk_try(vkGetPhysicalDeviceImageFormatProperties(m_physicalDevices[m_mainPDIndex], 
			image.format, image.type, image.tiling, image.usage, image.flags, &imageFormatProps));
		OutImageFormatProps.push_back(imageFormatProps);
	}
}
