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

	// Key, VkFormat, Size.
	const std::unordered_map<std::string, std::tuple<VkFormat, uint32> > VertexAttributeMap =
	{
		{ "position",  std::make_tuple(VK_FORMAT_R32G32B32_SFLOAT,  12u) },
		{ "color",     std::make_tuple(VK_FORMAT_R8G8B8A8_UNORM,     4u) },
		{ "color8u",   std::make_tuple(VK_FORMAT_R8G8B8A8_UINT,      4u) },
		{ "color32",   std::make_tuple(VK_FORMAT_R32G32B32_SFLOAT,  12u) },
		{ "normal",    std::make_tuple(VK_FORMAT_R32G32B32_SFLOAT,  12u) },
		{ "tangent",   std::make_tuple(VK_FORMAT_R32G32B32_SFLOAT,  12u) },
		{ "uv",        std::make_tuple(VK_FORMAT_R32G32_SFLOAT,      8u) }
	};

	const std::unordered_map<std::string, VkPrimitiveTopology> PrimitiveTopologyMap =
	{
		{ "point_list",          VK_PRIMITIVE_TOPOLOGY_POINT_LIST                    },
		{ "line_list",           VK_PRIMITIVE_TOPOLOGY_LINE_LIST                     },
		{ "line_strip",          VK_PRIMITIVE_TOPOLOGY_LINE_STRIP                    },
		{ "triangle_list",       VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST                 },
		{ "triangle_strip",      VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP                },
		{ "triangle_fan",        VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN                  },
		{ "line_list_adj",       VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY      },
		{ "line_strip_adj",      VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY     },
		{ "triangle_list_adj",   VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY  },
		{ "triangle_strip_adj",  VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY },
		{ "patch_list",          VK_PRIMITIVE_TOPOLOGY_PATCH_LIST                    }
	};

	const std::string DefaultShaderEntryPoint  = "main";
	const std::string DefaultPrimitiveTopology = "triangle_list";

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

	inline VkFormat GetVertexAttributeVkFormat(const std::string& InKey)
	{
		VkFormat result;
		try
		{
			result = std::get<0>(Util::VertexAttributeMap.at(InKey));
			return result;
		}
		catch (const std::out_of_range& msg)
		{
			result = VK_FORMAT_UNDEFINED;
			_returnx_log(result, std::string(msg.what()) + ", Func: " + _str_name_of(GetVertexAttributeVkFormat) + " input is not valid!");
		}
	}

	inline uint32 GetVertexAttributeSize(const std::string& InKey)
	{
		uint32 result;
		try
		{
			result = std::get<1>(Util::VertexAttributeMap.at(InKey));
			return result;
		}
		catch (const std::out_of_range& msg)
		{
			result = 0;
			_returnx_log(result, std::string(msg.what()) + ", Func: " + _str_name_of(GetVertexAttributeSize) + " input is not valid!");
		}
	}

	inline VkPrimitiveTopology GetPrimitiveTopology(const std::string& InKey)
	{
		VkPrimitiveTopology result;
		try
		{
			result = Util::PrimitiveTopologyMap.at(InKey);
			return result;
		}
		catch (const std::out_of_range& msg)
		{
			result = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			_returnx_log(result, std::string(msg.what()) + ", pipeline input assembly, primitive topology invalid! default set to \"" + Util::DefaultPrimitiveTopology + "\"!");
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