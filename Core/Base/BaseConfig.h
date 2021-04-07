/*********************************************************************
 *  BaseConfig.h
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 * 
 *  .
 *********************************************************************/

#pragma once

#include "Core/Common.h"

namespace BaseConfig
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
		//"VK_LAYER_RENDERDOC_Capture",
		"VK_LAYER_VALVE_steam_fossilize"
	};

	struct SwapchainCreateInfo
	{
		uint32                        frameCount;
		VkSurfaceFormatKHR            surfaceFormat;
		VkImageUsageFlags             imageUsage;
		VkSurfaceTransformFlagBitsKHR surfacePreTransform;
		VkCompositeAlphaFlagBitsKHR   compositeAlpha;
		VkPresentModeKHR              presentMode;
		VkBool32                      clipped;
	};

	static const SwapchainCreateInfo DefaultSwapchainCreateInfo =
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