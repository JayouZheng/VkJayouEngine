//
// Global.h
//

#pragma once

#include "LogicalDevice.h"

class BaseAllocator;

namespace Global
{
	VkInstance          GetVkInstance();
	void                SetVkInstance(const VkInstance& InInstance);

	LogicalDevice       GetLogicalDevice();
	void                SetLogicalDevice(const LogicalDevice& InLogicalDevice);

	VkDevice            GetVkDevice();
	void                SetVkDevice(const VkDevice& InDevice);

	void                Decrease();
	void                Advance();

	bool                IsZero();

	void                ApplicationDestroyManually(bool InFlag);
	bool                IsDestroyManually();

	BaseAllocator*           GetAllocator();
	VkAllocationCallbacks*   GetVkAllocator();
	void                     SetAllocator(BaseAllocator* InAllocator);
	void                     SafeFreeAllocator();
}

namespace VkColor
{
	const float Black[4] = { 0.0f,0.0f,0.0f,1.0f };
	const float White[4] = { 1.0f,1.0f,1.0f,1.0f };

	const float Red[4] = { 1.0f,0.0f,0.0f,1.0f };
	const float Green[4] = { 0.0f,1.0f,0.0f,1.0f };
	const float Blue[4] = { 0.0f,0.0f,1.0f,1.0f };
}

namespace Util
{
	const VkImageSubresourceRange ColorSubresRange =
	{
		VK_IMAGE_ASPECT_COLOR_BIT,   // aspectMask
		0,                           // baseMipLevel
		VK_REMAINING_MIP_LEVELS,     // levelCount
		0,                           // baseArrayLayer
		VK_REMAINING_ARRAY_LAYERS    // layerCount
	};

	const VkImageSubresourceRange DepthStencilSubresRange =
	{
		VK_IMAGE_ASPECT_DEPTH_BIT | 
		VK_IMAGE_ASPECT_STENCIL_BIT, // aspectMask
		0,                           // baseMipLevel
		VK_REMAINING_MIP_LEVELS,     // levelCount
		0,                           // baseArrayLayer
		VK_REMAINING_ARRAY_LAYERS    // layerCount
	};

	template<typename TType, typename TLambda>
	bool IsVecContain(const std::vector<TType>& InVecContain, const TType& InMember, const TLambda& InCompare)
	{
		for (auto& member : InVecContain)
		{
			if (InCompare(member, InMember))
			{
				return true;
			}
		}
		return false;
	}
}
