/*********************************************************************
 *  BaseLayer.cpp
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 *********************************************************************/

#include "BaseLayer.h"
#include "BaseConfig.h"
#include "BaseAllocator.h"
#include "Core/Engine/Engine.h"
#include "Core/Utility/Misc/Misc.h"
#include "Core/Platform/Windows/Window.h"
#include "Core/Render/RenderBase/LogicalDevice.h"

string GTestJaonPath;

_impl_create_interface(BaseLayer)

BaseLayer::BaseLayer() : 
	m_pAllocator  (nullptr),
	m_mainPDIndex (-1),
	m_mainQFIndex (-1)
{
	_zero_memory_struct(m_requiredPDFeatures);
	_zero_memory_struct(m_requiredPDVk12Features);
	_zero_memory_struct(m_surfaceCapabilities);
	_zero_memory_struct(m_swapchainCreateInfo);

	m_pDevice = LogicalDevice::Create(this);
	m_pWindow = Window::Create(this);

	GTestJaonPath = PathParser::Parse("Json/graphic_pipeline_info.json");
}

BaseLayer::~BaseLayer()
{
	Free();
}

bool BaseLayer::Init()
{
	_enable_runtime_memory_leak_check();

	CachedModulePath();

	// Global Variable.
	uint32 numEnableExts   = _array_size(BaseConfig::EnableExtensions);
	uint32 numEnableLayers = _array_size(BaseConfig::EnableLayers);

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
					if (_is_cstr_equal(prop.extensionName, BaseConfig::EnableExtensions[i]))
					{
						m_supportInsExts.push_back(BaseConfig::EnableExtensions[i]);
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
					if (_is_cstr_equal(prop.layerName, BaseConfig::EnableLayers[i]))
					{
						m_supportInsLayers.push_back(BaseConfig::EnableLayers[i]);
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

		// Enable Instance Exts.
		{
			uint32 numInsSupportExts = (uint32)m_supportInsExts.size();
			instanceCreateInfo.enabledExtensionCount   = numInsSupportExts;
			instanceCreateInfo.ppEnabledExtensionNames = numInsSupportExts > 0 ? m_supportInsExts.data() : nullptr;
		}

		// Enable Instance Layers.
		{
			uint32 numInsSupportLayers = (uint32)m_supportInsLayers.size();
			instanceCreateInfo.enabledLayerCount   = numInsSupportLayers;
			instanceCreateInfo.ppEnabledLayerNames = numInsSupportLayers > 0 ? m_supportInsLayers.data() : nullptr;
		}

		VkInstance vkInstance = VK_NULL_HANDLE;
		_vk_try(vkCreateInstance(&instanceCreateInfo, GetVkAllocator(), &vkInstance));
		SetVkInstance(vkInstance);

		// Enum physical devices.
		uint32 physicalDeviceCount = _count_0;
		_vk_try(vkEnumeratePhysicalDevices(GetVkInstance(), &physicalDeviceCount, nullptr));

		if (physicalDeviceCount == 0)
		{
			_log_error("Can't find any physical devices on the host!", LogSystem::Category::BaseLayer);
			return false;
		}

		m_physicalDevices.resize(physicalDeviceCount);
		_vk_try(vkEnumeratePhysicalDevices(GetVkInstance(), &physicalDeviceCount, m_physicalDevices.data()));

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

		if (m_mainPDIndex == -1)
		{
			_log_error("Can't find any valid physical devices on the host!", LogSystem::Category::BaseLayer);
			return false;
		}
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

		if (m_mainQFIndex == -1)
		{
			_log_error("Can't find any Valid Graphic & Compute Queue Family!", LogSystem::Category::BaseLayer);
			return false;
		}
		
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
					if (_is_cstr_equal(prop.extensionName, BaseConfig::EnableExtensions[i]))
					{
						m_supportPDExts.push_back(BaseConfig::EnableExtensions[i]);
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
					if (_is_cstr_equal(prop.layerName, BaseConfig::EnableLayers[i]))
					{
						m_supportPDLayers.push_back(BaseConfig::EnableLayers[i]);
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

		// Enable PD Exts.
		{
			uint32 numPDSupportExts = (uint32)m_supportPDExts.size();
			deviceCreateInfo.enabledExtensionCount   = numPDSupportExts;
			deviceCreateInfo.ppEnabledExtensionNames = numPDSupportExts > 0 ? m_supportPDExts.data() : nullptr;
		}

		// Enable PD Layers.
		{
			uint32 numPDSupportLayers = (uint32)m_supportPDLayers.size();
			deviceCreateInfo.enabledLayerCount   = numPDSupportLayers;
			deviceCreateInfo.ppEnabledLayerNames = numPDSupportLayers > 0 ? m_supportPDLayers.data() : nullptr;
		}

		_vk_try(vkCreateDevice(m_physicalDevices[m_mainPDIndex], &deviceCreateInfo, GetVkAllocator(), m_pDevice->GetAddressOfVkDevice()));
		m_pDevice->Init(this);
		SetVkDevice(m_pDevice->GetVkDevice());

		m_pDevice->CreateCommandPool(m_mainQFIndex);

#if PLATFORM_WINDOW

		// Create Win32 Surface.
		if (Misc::IsVecContain<const char*>(m_supportInsExts, VK_KHR_WIN32_SURFACE_EXTENSION_NAME, _lambda_is_cstr_equal))
		{
			VkBool32 bIsDefaultQueueSupportPresentation = vkGetPhysicalDeviceWin32PresentationSupportKHR(m_physicalDevices[m_mainPDIndex], m_mainQFIndex);

			if (bIsDefaultQueueSupportPresentation == VK_FALSE)
			{
				_log_error("The Default Queue Do Not Support Presentation (Win32)!", LogSystem::Category::BaseLayer);
				return false;
			}
			
			if (!m_pWindow->Init()) return false;
			VkWin32SurfaceCreateInfoKHR win32SurfaceCreateInfo = {};
			win32SurfaceCreateInfo.sType = VK_STRUCTURE_TYPE_DISPLAY_SURFACE_CREATE_INFO_KHR;
			win32SurfaceCreateInfo.hinstance = (HINSTANCE)m_pWindow->GetHinstance();
			win32SurfaceCreateInfo.hwnd = (HWND)m_pWindow->GetHwnd();

			_vk_try(vkCreateWin32SurfaceKHR(GetVkInstance(), &win32SurfaceCreateInfo, GetVkAllocator(), m_pSurface.MakeInstance()));
		}
		
#endif

		// Create Swapchain. // OnResize Recreate Needed!!!
		if (Misc::IsVecContain<const char*>(m_supportPDExts, VK_KHR_SWAPCHAIN_EXTENSION_NAME, _lambda_is_cstr_equal))
		{
			m_swapchainCreateInfo.sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			m_swapchainCreateInfo.surface               = *m_pSurface;
			m_swapchainCreateInfo.imageArrayLayers      = 1;        // Only 1 Layer.
			m_swapchainCreateInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
			m_swapchainCreateInfo.queueFamilyIndexCount = 0;
			m_swapchainCreateInfo.pQueueFamilyIndices   = nullptr;
			m_swapchainCreateInfo.presentMode           = BaseConfig::DefaultSwapchainCreateInfo.presentMode;
			m_swapchainCreateInfo.clipped               = BaseConfig::DefaultSwapchainCreateInfo.clipped;
			m_swapchainCreateInfo.oldSwapchain          = VK_NULL_HANDLE;  // First time to Create.

			// Check Support Surface Format For Swapchain.
			{
				uint32 formatCount = _count_0;
				_vk_try(vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevices[m_mainPDIndex], *m_pSurface, &formatCount, nullptr));

				if (formatCount == 0)
				{
					_log_error("No Surface Format Support!", LogSystem::Category::BaseLayer);
					return false;
				}

				m_surfaceFormats.resize(formatCount);
				_vk_try(vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevices[m_mainPDIndex], *m_pSurface, &formatCount, m_surfaceFormats.data()));

				m_swapchainCreateInfo.imageFormat = m_surfaceFormats.front().format;
				m_swapchainCreateInfo.imageColorSpace = m_surfaceFormats.front().colorSpace;

				if (Misc::IsVecContain<VkSurfaceFormatKHR>(
					m_surfaceFormats, 
					BaseConfig::DefaultSwapchainCreateInfo.surfaceFormat, 
					[&](const VkSurfaceFormatKHR& a, const VkSurfaceFormatKHR& b) { return (a.format == b.format) && (a.colorSpace == b.colorSpace); }))
				{
					m_swapchainCreateInfo.imageFormat = BaseConfig::DefaultSwapchainCreateInfo.surfaceFormat.format;
					m_swapchainCreateInfo.imageColorSpace = BaseConfig::DefaultSwapchainCreateInfo.surfaceFormat.colorSpace;
				}
			}

			// Query Surface Capabilities.
			{
				_vk_try(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevices[m_mainPDIndex], *m_pSurface, &m_surfaceCapabilities));

				m_swapchainCreateInfo.imageExtent = m_surfaceCapabilities.currentExtent;

				if ((BaseConfig::DefaultSwapchainCreateInfo.frameCount > m_surfaceCapabilities.minImageCount) && 
					(BaseConfig::DefaultSwapchainCreateInfo.frameCount < m_surfaceCapabilities.maxImageCount))
					m_swapchainCreateInfo.minImageCount = BaseConfig::DefaultSwapchainCreateInfo.frameCount;
				else m_swapchainCreateInfo.minImageCount = m_surfaceCapabilities.minImageCount;

				if (!(m_surfaceCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT))
				{
					_log_error("Surface Do Not Support Color Attachment Usage!", LogSystem::Category::BaseLayer);
					return false;
				}

				m_swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT & BaseConfig::DefaultSwapchainCreateInfo.imageUsage;

				if (BaseConfig::DefaultSwapchainCreateInfo.surfacePreTransform & m_surfaceCapabilities.supportedTransforms)
					m_swapchainCreateInfo.preTransform = BaseConfig::DefaultSwapchainCreateInfo.surfacePreTransform;
				else m_swapchainCreateInfo.preTransform = m_surfaceCapabilities.currentTransform;

				if (BaseConfig::DefaultSwapchainCreateInfo.compositeAlpha & m_surfaceCapabilities.supportedCompositeAlpha)
					m_swapchainCreateInfo.compositeAlpha = BaseConfig::DefaultSwapchainCreateInfo.compositeAlpha;
				else m_swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			}
			
			m_pDevice->CreateSwapchainKHR(m_pSwapchainKHR.MakeInstance(), m_swapchainCreateInfo);

			uint32 swapchainImageCount = _count_0;
			m_pDevice->GetSwapchainImagesKHR(*m_pSwapchainKHR, &swapchainImageCount, nullptr);
			m_swapchainImages.resize(swapchainImageCount);
			m_pDevice->GetSwapchainImagesKHR(*m_pSwapchainKHR, &swapchainImageCount, m_swapchainImages.data());

			// This Code May Be Redundant... Win32 Surface Presentation Support Has Been Queried.
			{
				VkBool32 surfacePresentationSupport = VK_FALSE;
				_vk_try(vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevices[m_mainPDIndex], m_mainQFIndex, *m_pSurface, &surfacePresentationSupport));

				if (surfacePresentationSupport == VK_FALSE)
				{
					_log_error("The Default Queue Do Not Support Presentation!", LogSystem::Category::BaseLayer);
					return false;
				}
			}
			
		}
		else
		{
			_log_error("Create Swapchain Failed! Application Terminate!", LogSystem::Category::BaseLayer);
			return false;
		}

		// TODO:
		m_pDevice->CreateGraphicPipelines(GTestJaonPath);
		
		m_pWindow->Show();
	}

	return true;
}

void BaseLayer::CachedModulePath()
{
#if PLATFORM_WINDOW

	string modulePath = ""; // It needs to be left empty here.
	string moduleName = "null";

	char exePath[_max_path];

	GetModuleFileNameA(NULL, exePath, _max_path);

	modulePath = string(exePath);
	string::size_type found = modulePath.find_last_of("\\/");
	if (found != string::npos)
	{
		moduleName = modulePath.substr(found + 1);
		modulePath = modulePath.substr(0, found + 1);
	}

	// Add path that will remove form modulePath.
	std::vector<string> deleteDirs =
	{
		"x64\\",
		"Debug\\",
		"Release\\"
	};

	for (auto& dir : deleteDirs)
	{
		found = string::npos;
		found = modulePath.rfind(dir);
		if (found != string::npos)
			modulePath.erase(found, dir.size());
	}

	Engine::ModuleInfo moduleInfo;
	moduleInfo.Path = modulePath;
	moduleInfo.Name = moduleName;
	Engine::Get()->CacheModuleInfo(moduleInfo);

#endif
}

void BaseLayer::SetBaseAllocator(BaseAllocator* InAllocator)
{
	m_pAllocator = InAllocator;
	VkSmartPtr_Private::SetBaseAllocator(InAllocator);
}

void BaseLayer::SafeFreeAllocator()
{
	if (m_pAllocator != nullptr)
	{
		delete m_pAllocator;
		m_pAllocator = nullptr;
	}
}

void BaseLayer::SetVkInstance(const VkInstance& InInstance)
{
	m_instance = InInstance;
	VkSmartPtr_Private::SetVkInstance(InInstance);
}

void BaseLayer::SetVkDevice(const VkDevice& InDevice)
{
	m_device = InDevice;
	VkSmartPtr_Private::SetVkDevice(InDevice);
}

void BaseLayer::Free()
{
}

VkInstance BaseLayer::GetVkInstance() const
{
	return m_instance;
}

VkDevice BaseLayer::GetVkDevice() const
{
	return m_device;
}

BaseAllocator* BaseLayer::GetBaseAllocator() const
{
	return m_pAllocator;
}

VkAllocationCallbacks* BaseLayer::GetVkAllocator() const
{
	return m_pAllocator != nullptr ? m_pAllocator->GetVkAllocator() : nullptr;
}

Window* BaseLayer::GetWindow() const
{
	return m_pWindow;
}

LogicalDevice* BaseLayer::GetLogicalDevice() const
{
	return m_pDevice;
}

const VkPhysicalDeviceLimits& BaseLayer::GetMainPDLimits() const
{
	return m_physicalDevicesProps[m_mainPDIndex].limits;
}

const VkPhysicalDeviceProperties& BaseLayer::GetMainPDProps() const
{
	return m_physicalDevicesProps[m_mainPDIndex];
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

void BaseLayer::CheckImageFormatSupport(const std::vector<CheckImageSupportInfo>& InCheckImages, std::vector<VkImageFormatProperties>& OutImageFormatProps)
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
