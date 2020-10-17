//
// Global.h
//

#pragma once

#include "Common.h"

class BaseAllocator;

namespace Global
{
	struct ModuleInfo
	{
		std::string Path;
		std::string Name;
	};

	VkInstance              GetVkInstance();
	void                    SetVkInstance(const VkInstance& InInstance);

	VkDevice                GetVkDevice();
	void                    SetVkDevice(const VkDevice& InDevice);

	void                    Decrease();
	void                    Advance();

	bool                    IsZero();

	void                    ApplicationDestroyManually(bool InFlag);
	bool                    IsDestroyManually();

	void                    CacheModuleInfo(const ModuleInfo& InModuleInfo);
	std::string             GetModulePath();
	std::string             GetModuleName();

	void                    CacheLog(const std::string& InLog);

	BaseAllocator*          GetAllocator();
	VkAllocationCallbacks*  GetVkAllocator();
	void                    SetAllocator(BaseAllocator* InAllocator);
	void                    SafeFreeAllocator();

	void                    OnExit();

	template<typename T>
	bool IsVkGuaranteedMinimum(T InValue, T InMinimum)
	{
		return InValue >= InMinimum;
	}
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

	const std::string DefaultShaderEntryPoint   = "main";
	const std::string DefaultPrimitiveTopology  = "triangle_list";
	const std::string DefaultPolygonMode        = "fill";
	const std::string DefaultCullMode           = "cull_none";
	const std::string DefaultFrontFace          = "counter_clockwise";
	const std::string DefaultCompareOp          = "less_equal";
	const std::string DefaultStencilOp          = "keep";
	const std::string DefaultLogicOp            = "clear";
	const std::string DefaultBlendFactor        = "zero";
	const std::string DefaultBlendOp            = "add";
	const std::string DefaultColorComponentMask = "rgba";
	const std::string DefaultDynamicState       = "viewport";

	bool                     GetShaderStage             (const std::string& InKey, VkShaderStageFlagBits& OutShaderStage);
	VkFormat                 GetVertexAttributeVkFormat (const std::string& InKey);
	uint32                   GetVertexAttributeSize     (const std::string& InKey);
	VkPrimitiveTopology      GetPrimitiveTopology       (const std::string& InKey);
	VkPolygonMode            GetPolygonMode             (const std::string& InKey);
	VkCullModeFlagBits       GetCullMode                (const std::string& InKey);
	VkFrontFace              GetFrontFace               (const std::string& InKey);
	VkSampleCountFlagBits    GetMultisampleCount        (uint32 InCount);
	VkCompareOp              GetCompareOp               (const std::string& InKey);
	VkStencilOp              GetStencilOp               (const std::string& InKey);
	VkLogicOp                GetLogicOp                 (const std::string& InKey);
	VkBlendFactor            GetBlendFactor             (const std::string& InKey);
	VkBlendOp                GetBlendOp                 (const std::string& InKey);
	VkColorComponentFlags    GetColorComponentMask      (const std::string& InKey);
	VkDynamicState           GetDynamicState            (const std::string& InKey);

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

	inline void PrintArgs(const char* InFormat) { std::cout << InFormat; } // Base function

	template<typename T, typename... Targs>
	void PrintArgs(const char* InFormat, T InValue, Targs... InFargs)
	{
		for (; *InFormat != '\0'; InFormat++) {
			if (*InFormat == '%') {
				std::cout << InValue;
				PrintArgs(InFormat + 1, InFargs...); // Recursive call
				return;
			}
			std::cout << *InFormat;
		}
	}

	bool ParseJson(const std::string& InPath, Json::Value& OutRoot);

	uint32 StringToHex(const std::string& InHexStr);
}

// TODO:
// The log system has been added, these code block should be changed as soon as possible.

#define _return_log(log) { Global::CacheLog(log); return; }
#define _breturn_log(b, log) if (b) { Global::CacheLog(log); return; }
#define _returnx_log(ret, log) { Global::CacheLog(log); return ret; }
#define _breturnx_log(b, ret, log) if(b) { Global::CacheLog(log); return ret; }
#define _ret_false_log(log) _returnx_log(false, log)
#define _bret_false_log(b, log) _breturnx_log(b, false, log)

#define _bcontinue_log(b, log) if (b) { Global::CacheLog(log); continue; }
#define _bbreak_log(b, log) if (b) { Global::CacheLog(log); break; }

#define _is_guaranteed_min(x, min_val, y) { if (Global::IsVkGuaranteedMinimum<uint32>(x, min_val)) x = std::min(x, y); }
