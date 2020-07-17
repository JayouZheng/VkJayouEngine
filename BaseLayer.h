//
// BaseLayer.h
//

#pragma once

#include "vk_util.h"
#include "VkSmartPtr.h"
#include "BaseAllocator.h"

class Window;

namespace BaseLayerConfig
{
	static const char* EnableExtensions[] = 
	{ 
		// VK_KHR_SURFACE_EXTENSION_NAME, 
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME
	};
}

class BaseLayer
{

private:

	BaseAllocator* m_allocator = nullptr;

protected:

	// Physical device infos.
	std::vector<VkPhysicalDevice>                     m_physicalDevices;
	std::vector<VkPhysicalDeviceProperties>           m_physicalDevicesProps;
	std::vector<VkPhysicalDeviceFeatures>             m_physicalDevicesFeatures;
	std::vector<VkPhysicalDeviceMemoryProperties>     m_physicalDevicesMemProps;

	// Queue family props.
	std::vector<std::vector<VkQueueFamilyProperties>> m_queueFamilyProps;

	// Layers & Extensions.
	std::vector<VkLayerProperties>                    m_instanceLayerProps;
	std::vector<std::vector<VkLayerProperties>>       m_PDLayerProps;
	std::vector<VkExtensionProperties>                m_instanceExtProps;
	std::vector<std::vector<VkExtensionProperties>>   m_PDExtProps;

protected:

	VkQueue       m_queue = VK_NULL_HANDLE;
	VkSurfaceKHR  m_surface = VK_NULL_HANDLE;

	int32_t                                           m_defaultPDIndex = -1;
	int32_t                                           m_graphicQueueFamilyIndex = -1;
	VkPhysicalDeviceFeatures                          m_requiredPDFeatures = {};

	std::vector<const char*>                          m_supportEnableExts;

	Window*                                           m_window = nullptr;

protected:

	VkSmartPtr<VkCommandPool>                         m_pCmdPool;

public:

	BaseLayer();
	BaseLayer(BaseAllocator* InAllocator);
	virtual ~BaseLayer();

public:

	void Init();
	void Free();

public:

	uint32_t GetHeapIndexFromMemPropFlags(
		const VkMemoryRequirements& InMemRequirements,
		VkMemoryPropertyFlags InPreferredFlags,
		VkMemoryPropertyFlags InRequiredFlags);

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