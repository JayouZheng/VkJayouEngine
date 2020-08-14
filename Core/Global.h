//
// Global.h
//

#pragma once

#include "Common.h"

class BaseAllocator;

namespace Global
{
	VkInstance          GetVkInstance();
	void                SetVkInstance(const VkInstance& InInstance);

	VkDevice            GetVkDevice();
	void                SetVkDevice(const VkDevice& InDevice);

	void                Decrease();
	void                Advance();

	bool                IsZero();

	void                ApplicationDestroyManually(bool InFlag);
	bool                IsDestroyManually();

	void                CacheLog(const std::string& InLog);
	void                PrintLog();

	BaseAllocator*           GetAllocator();
	VkAllocationCallbacks*   GetVkAllocator();
	void                     SetAllocator(BaseAllocator* InAllocator);
	void                     SafeFreeAllocator();

	template<typename T>
	bool IsVkGuaranteedMinimum(T InValue, T InMinimum)
	{
		return InValue >= InMinimum;
	}
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

	const std::unordered_map<std::string, VkShaderStageFlagBits> ShaderStageMap =
	{
		{ "null",                    VK_SHADER_STAGE_VERTEX_BIT                  },
		{ "vertex",                  VK_SHADER_STAGE_VERTEX_BIT                  },
		{ "pixel",                   VK_SHADER_STAGE_FRAGMENT_BIT                },
		{ "fragment",                VK_SHADER_STAGE_FRAGMENT_BIT                },
		{ "tessellation_control",    VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT    },
		{ "tessellation_evaluation", VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT },
		{ "geometry",                VK_SHADER_STAGE_GEOMETRY_BIT                },
		{ "compute",                 VK_SHADER_STAGE_COMPUTE_BIT                 },
		{ "mesh",                    VK_SHADER_STAGE_MESH_BIT_NV                 },
		{ "raygen",                  VK_SHADER_STAGE_RAYGEN_BIT_NV               }
	};

	inline VkShaderStageFlagBits GetShaderStage(const std::string& InKey)
	{
		VkShaderStageFlagBits result;
		try
		{
			result = Util::ShaderStageMap.at(InKey);
			return result;
		}
		catch (const std::out_of_range& msg)
		{
			result = VK_SHADER_STAGE_VERTEX_BIT;
			_returnx_log(result, std::string(msg.what()) + ", pipeline stage type invalid! default set to \"vertex\"!");
		}
	}

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

	void PrintArgs(const char* InFormat); // base function

	template<typename T, typename... Targs>
	void PrintArgs(const char* InFormat, T InValue, Targs... InFargs) // recursive variadic function
	{
		for (; *InFormat != '\0'; InFormat++) {
			if (*InFormat == '%') {
				std::cout << InValue;
				PrintArgs(InFormat + 1, InFargs...); // recursive call
				return;
			}
			std::cout << *InFormat;
		}
	}


	bool ParseJson(const std::string& InPath, Json::Value& OutRoot);
}