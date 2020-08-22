//
// Global.cpp
//

#include "Global.h"
#include "BaseAllocator.h"

namespace
{
	static VkInstance     g_instance         = VK_NULL_HANDLE;
	static VkDevice       g_device           = VK_NULL_HANDLE;

	static BaseAllocator* g_allocator        = nullptr;

	static uint32         g_count            = _count_0;
	static bool           g_bDestroyManually = false;


	static std::vector<std::string>          g_logs;


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

	const std::unordered_map<std::string, VkPolygonMode> PolygonModeMap =
	{
		{ "fill",   VK_POLYGON_MODE_FILL  },
		{ "line",   VK_POLYGON_MODE_LINE  },
		{ "point",  VK_POLYGON_MODE_POINT }
	};

	const std::unordered_map<std::string, VkCullModeFlagBits> CullModeMap =
	{
		{ "cull_none",   VK_CULL_MODE_NONE           },
		{ "cull_front",  VK_CULL_MODE_FRONT_BIT      },
		{ "cull_back",   VK_CULL_MODE_BACK_BIT       },
		{ "cull_both",   VK_CULL_MODE_FRONT_AND_BACK }
	};

	const std::unordered_map<std::string, VkFrontFace> FrontFaceMap =
	{
		{ "counter_clockwise",  VK_FRONT_FACE_COUNTER_CLOCKWISE },
		{ "clockwise",          VK_FRONT_FACE_CLOCKWISE         }
	};

	const std::unordered_map<std::string, VkCompareOp> CompareOpMap =
	{
		{ "never",			   VK_COMPARE_OP_NEVER            },
		{ "less",			   VK_COMPARE_OP_LESS			  },
		{ "<",				   VK_COMPARE_OP_LESS			  },
		{ "equal",			   VK_COMPARE_OP_EQUAL			  },
		{ "==",				   VK_COMPARE_OP_EQUAL			  },
		{ "less_equal",  	   VK_COMPARE_OP_LESS_OR_EQUAL	  },
		{ "<=",	               VK_COMPARE_OP_LESS_OR_EQUAL	  },
		{ "greater",		   VK_COMPARE_OP_GREATER		  },
		{ ">",       		   VK_COMPARE_OP_GREATER		  },
		{ "not_equal",		   VK_COMPARE_OP_NOT_EQUAL		  },
		{ "!=",		           VK_COMPARE_OP_NOT_EQUAL		  },
		{ "greater_equal",     VK_COMPARE_OP_GREATER_OR_EQUAL },
		{ ">=",                VK_COMPARE_OP_GREATER_OR_EQUAL },
		{ "always",			   VK_COMPARE_OP_ALWAYS			  }
	};

	const std::unordered_map<std::string, VkStencilOp> StencilOpMap =
	{
		{"keep",             VK_STENCIL_OP_KEEP                },
		{"zero",             VK_STENCIL_OP_ZERO                },
		{"replace",          VK_STENCIL_OP_REPLACE             },
		{"increment_clamp",  VK_STENCIL_OP_INCREMENT_AND_CLAMP },
		{"++clamp",          VK_STENCIL_OP_INCREMENT_AND_CLAMP },
		{"decrement_clamp",  VK_STENCIL_OP_DECREMENT_AND_CLAMP },
		{"--clamp",          VK_STENCIL_OP_DECREMENT_AND_CLAMP },
		{"invert",           VK_STENCIL_OP_INVERT              },
		{"increment_wrap",   VK_STENCIL_OP_INCREMENT_AND_WRAP  },
		{"++wrap",           VK_STENCIL_OP_INCREMENT_AND_WRAP  },
		{"decrement_wrap",   VK_STENCIL_OP_DECREMENT_AND_WRAP  },
		{"--wrap",           VK_STENCIL_OP_DECREMENT_AND_WRAP  }
	};
}

VkInstance Global::GetVkInstance()
{
	_bexit_log(g_instance == VK_NULL_HANDLE, "return [g_instance] is null!");
	return g_instance;
}

void Global::SetVkInstance(const VkInstance& InInstance)
{
	g_instance = InInstance;
}

VkDevice Global::GetVkDevice()
{
	_bexit_log(g_device == VK_NULL_HANDLE, "return [g_device] is null!");
	return g_device;
}

void Global::SetVkDevice(const VkDevice& InDevice)
{
	g_device = InDevice;
}

void Global::Decrease()
{
	g_count--;
}

void Global::Advance()
{
	g_count++;
}

bool Global::IsZero()
{
	return g_count == 0;
}

void Global::ApplicationDestroyManually(bool InFlag)
{
	g_bDestroyManually = InFlag;
}

bool Global::IsDestroyManually()
{
	return g_bDestroyManually;
}

void Global::CacheLog(const std::string& InLog)
{
	g_logs.push_back(InLog);
}

void Global::PrintLog()
{
	uint32 logID = 0;
	for (auto& log : g_logs)
	{
		_cmd_print_line("Global Cached " + std::to_string(g_logs.size()) + " Logs:");
		_cmd_print_line(std::to_string(logID++) + ". " + log);
	}
}

BaseAllocator* Global::GetAllocator()
{
	return g_allocator;
}

VkAllocationCallbacks* Global::GetVkAllocator()
{
	return g_allocator->GetVkAllocator();
}

void Global::SetAllocator(BaseAllocator* InAllocator)
{
	g_allocator = InAllocator;
}

void Global::SafeFreeAllocator()
{
	if (g_allocator != nullptr)
	{
		delete g_allocator;
		g_allocator = nullptr;
	}
}

VkShaderStageFlagBits Util::GetShaderStage(const std::string& InKey)
{
	VkShaderStageFlagBits result;
	try
	{
		result = ShaderStageMap.at(InKey);
		return result;
	}
	catch (const std::out_of_range& msg)
	{
		result = VK_SHADER_STAGE_VERTEX_BIT;
		_returnx_log(result, std::string(msg.what()) + ", pipeline stage type invalid! default set to \"vertex\"!");
	}
}

VkFormat Util::GetVertexAttributeVkFormat(const std::string& InKey)
{
	VkFormat result;
	try
	{
		result = std::get<0>(VertexAttributeMap.at(InKey));
		return result;
	}
	catch (const std::out_of_range& msg)
	{
		result = VK_FORMAT_UNDEFINED;
		_returnx_log(result, std::string(msg.what()) + ", Func: " + _str_name_of(GetVertexAttributeVkFormat) + " input is not valid!");
	}
}

uint32 Util::GetVertexAttributeSize(const std::string& InKey)
{
	uint32 result;
	try
	{
		result = std::get<1>(VertexAttributeMap.at(InKey));
		return result;
	}
	catch (const std::out_of_range& msg)
	{
		result = 0;
		_returnx_log(result, std::string(msg.what()) + ", Func: " + _str_name_of(GetVertexAttributeSize) + " input is not valid!");
	}
}

VkPrimitiveTopology Util::GetPrimitiveTopology(const std::string& InKey)
{
	VkPrimitiveTopology result;
	try
	{
		result = PrimitiveTopologyMap.at(InKey);
		return result;
	}
	catch (const std::out_of_range& msg)
	{
		result = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		_returnx_log(result, std::string(msg.what()) + ", pipeline input assembly, primitive topology invalid! default set to \"" + Util::DefaultPrimitiveTopology + "\"!");
	}
}

VkPolygonMode Util::GetPolygonMode(const std::string& InKey)
{
	VkPolygonMode result;
	try
	{
		result = PolygonModeMap.at(InKey);
		return result;
	}
	catch (const std::out_of_range& msg)
	{
		result = VK_POLYGON_MODE_FILL;
		_returnx_log(result, std::string(msg.what()) + ", pipeline rasterization state, polygon mode invalid! default set to \"" + Util::DefaultPolygonMode + "\"!");
	}
}

VkCullModeFlagBits Util::GetCullMode(const std::string& InKey)
{
	VkCullModeFlagBits result;
	try
	{
		result = CullModeMap.at(InKey);
		return result;
	}
	catch (const std::out_of_range& msg)
	{
		result = VK_CULL_MODE_NONE;
		_returnx_log(result, std::string(msg.what()) + ", pipeline rasterization state, cull mode invalid! default set to \"" + Util::DefaultCullMode + "\"!");
	}
}

VkFrontFace Util::GetFrontFace(const std::string& InKey)
{
	VkFrontFace result;
	try
	{
		result = FrontFaceMap.at(InKey);
		return result;
	}
	catch (const std::out_of_range& msg)
	{
		result = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		_returnx_log(result, std::string(msg.what()) + ", pipeline rasterization state, front face invalid! default set to \"" + Util::DefaultFrontFace + "\"!");
	}
}

VkSampleCountFlagBits Util::GetMultisampleCount(uint32 InCount)
{
	if (InCount <= 1u)
		return VK_SAMPLE_COUNT_1_BIT;
	if (InCount <= 2u)
		return VK_SAMPLE_COUNT_2_BIT;
	if (InCount <= 4u)
		return VK_SAMPLE_COUNT_4_BIT;
	if (InCount <= 8u)
		return VK_SAMPLE_COUNT_8_BIT;
	if (InCount <= 16u)
		return VK_SAMPLE_COUNT_16_BIT;
	if (InCount <= 32u)
		return VK_SAMPLE_COUNT_32_BIT;
	if (InCount <= 64u)
		return VK_SAMPLE_COUNT_64_BIT;
	if (InCount > 64u)
		return VK_SAMPLE_COUNT_64_BIT;
	return VK_SAMPLE_COUNT_1_BIT;
}

VkCompareOp Util::GetCompareOp(const std::string& InKey)
{
	VkCompareOp result;
	try
	{
		result = CompareOpMap.at(InKey);
		return result;
	}
	catch (const std::out_of_range& msg)
	{
		result = VK_COMPARE_OP_LESS_OR_EQUAL;
		_returnx_log(result, std::string(msg.what()) + ", pipeline depth stencil state, compare op invalid! default set to \"" + Util::DefaultCompareOp + "\"!");
	}
}

VkStencilOp Util::GetStencilOp(const std::string& InKey)
{
	VkStencilOp result;
	try
	{
		result = StencilOpMap.at(InKey);
		return result;
	}
	catch (const std::out_of_range& msg)
	{
		result = VK_STENCIL_OP_KEEP;
		_returnx_log(result, std::string(msg.what()) + ", pipeline depth stencil state, stencil op invalid! default set to \"" + Util::DefaultStencilOp + "\"!");
	}
}

void Util::PrintArgs(const char* InFormat)
{
	std::cout << InFormat;
}

bool Util::ParseJson(const std::string& InPath, Json::Value& OutRoot)
{
	std::ifstream ifs(InPath, std::ifstream::in);
	
	if (ifs.is_open())
	{
		Json::CharReaderBuilder builder;
		JSONCPP_STRING errs;

		if (!parseFromStream(builder, ifs, &OutRoot, &errs))
		{
			_returnx_log(false, errs);
		}

		return true;
	}
	else
	{
		_returnx_log(false, "Error: Could not open Json file \"" + InPath + "\"");
	}
}

uint32 Util::StringToHex(const std::string& InHexStr)
{
	std::stringstream strs;
	strs << InHexStr;
	uint32 value;
	strs >> std::hex >> value;
	return value;
}
