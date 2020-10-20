//
// BaseLayer.h
//

#pragma once

#include "Common.h"
#include "VkSmartPtr.h"
#include "Window.h"
#include "LogicalDevice.h"
#include "CommandQueue.h"
#include "CommandList.h"

namespace BaseLayerConfig
{
	static const char* EnableExtensions[] = 
	{ 
		VK_KHR_SURFACE_EXTENSION_NAME, 
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
		VK_KHR_DISPLAY_EXTENSION_NAME,
		VK_KHR_DISPLAY_SWAPCHAIN_EXTENSION_NAME
	};

	static const char* EnableLayers[] =
	{
		"VK_LAYER_RENDERDOC_Capture",
		"VK_LAYER_VALVE_steam_fossilize"
	};

	struct SSwapchainCreateInfo
	{
		uint32                        frameCount;
		VkSurfaceFormatKHR            surfaceFormat;
		VkImageUsageFlags             imageUsage;
		VkSurfaceTransformFlagBitsKHR surfacePreTransform;
		VkCompositeAlphaFlagBitsKHR   compositeAlpha;
		VkPresentModeKHR              presentMode;
		VkBool32                      clipped;
	};

	static const SSwapchainCreateInfo SwapchainCreateInfo =
	{
		3, // 3 FrameResource Required!
		{ VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR },
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
		VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		VK_PRESENT_MODE_FIFO_KHR,              // Vertical Sync.
		VK_TRUE
	};
}

class BaseAllocator;

class BaseLayer
{
	// PD means Physical Device.
	// QF means Queue Family.

private:

	BaseAllocator* m_pAllocator = nullptr;

	VkAllocationCallbacks* GetVkAllocator() const;

protected:

	// Physical device infos.
	std::vector<VkPhysicalDevice>                     m_physicalDevices;
	std::vector<VkPhysicalDeviceProperties>           m_physicalDevicesProps;
	std::vector<VkPhysicalDeviceFeatures2>            m_physicalDevicesFeatures;
	std::vector<VkPhysicalDeviceVulkan12Features>     m_PDVulkan12Features;
	std::vector<VkPhysicalDeviceMemoryProperties>     m_physicalDevicesMemProps;

	// Queue family props.
	std::vector<std::vector<VkQueueFamilyProperties>> m_queueFamilyProps;

	// Layers & Extensions.
	std::vector<VkLayerProperties>                    m_instanceLayerProps;
	std::vector<std::vector<VkLayerProperties>>       m_PDLayerProps;
	std::vector<VkExtensionProperties>                m_instanceExtProps;
	std::vector<std::vector<VkExtensionProperties>>   m_PDExtProps;

	// Required PD Features.
	VkPhysicalDeviceFeatures2                         m_requiredPDFeatures     = {};
	VkPhysicalDeviceVulkan12Features                  m_requiredPDVk12Features = {};

	// Support Extensions.
	std::vector<const char*>                          m_supportInsExts;
	std::vector<const char*>                          m_supportPDExts;

	// Support Layers.
	std::vector<const char*>                          m_supportInsLayers;
	std::vector<const char*>                          m_supportPDLayers;

	// Surface Support Format KHR.
	std::vector<VkSurfaceFormatKHR>                   m_surfaceFormats;

	// Surface Capabilities KHR.
	VkSurfaceCapabilitiesKHR                          m_surfaceCapabilities;

	// Swapchain Create Info KHR.
	VkSwapchainCreateInfoKHR                          m_swapchainCreateInfo = {}; // Cached for using when window resize.

	// std::vector<VkDisplayPropertiesKHR>            m_displayProps;  // No Display...

protected:

	LogicalDevice  m_device;
	CommandQueue   m_queue;

	int32                                             m_mainPDIndex = -1;
	int32                                             m_mainQFIndex = -1;
	
	SmartPtr<Window>                                  m_pWindow = nullptr;

protected:

	_declare_vk_smart_ptr(VkSurfaceKHR,   m_pSurface);
	_declare_vk_smart_ptr(VkSwapchainKHR, m_pSwapchainKHR);

	// Swapchain Image.
	std::vector<VkImage>                              m_swapchainImages;

public:

	BaseLayer();
	BaseLayer(BaseAllocator* InAllocator);
	virtual ~BaseLayer();

public:

	bool Init();
	void CachedModulePath();

private:

	void Free();

public:

	uint32 GetHeapIndexFromMemPropFlags(
		const VkMemoryRequirements& InMemRequirements,
		VkMemoryPropertyFlags InPreferredFlags,
		VkMemoryPropertyFlags InRequiredFlags);

	LogicalDevice          GetLogicalDevice() const;
	VkPhysicalDeviceLimits GetMainPDLimits () const;

public:

	// Query physical device supported buffer/image formats.
	void CheckFormatSupport(const std::vector<VkFormat>& InCheckFormats, std::vector<VkFormatProperties>& OutFormatProps);

	struct SCheckImage
	{
		VkFormat           format;
		VkImageType        type;
		VkImageTiling      tiling;
		VkImageUsageFlags  usage;
		VkImageCreateFlags flags;
	};

	void CheckImageFormatSupport(const std::vector<SCheckImage>& InCheckImages, std::vector<VkImageFormatProperties>& OutImageFormatProps);
};