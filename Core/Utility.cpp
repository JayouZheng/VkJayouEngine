﻿//
// Utility.cpp
//

#include "Utility.h"
#include "StringManager.h"

namespace
{
	/// <summary>
	/// Utility String Map Object Tables.
	/// </summary>
	const std::unordered_map<std::string, VkShaderStageFlags> ShaderStageMap =
	{
		{ "vertex",                  VK_SHADER_STAGE_VERTEX_BIT                  },
		{ "pixel",                   VK_SHADER_STAGE_FRAGMENT_BIT                },
		{ "fragment",                VK_SHADER_STAGE_FRAGMENT_BIT                },
		{ "tessellation_control",    VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT    },
		{ "tessellation_evaluation", VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT },
		{ "geometry",                VK_SHADER_STAGE_GEOMETRY_BIT                },
		{ "compute",                 VK_SHADER_STAGE_COMPUTE_BIT                 },
		{ "mesh",                    VK_SHADER_STAGE_MESH_BIT_NV                 },
		{ "raygen",                  VK_SHADER_STAGE_RAYGEN_BIT_NV               },
		{ "vert",                    VK_SHADER_STAGE_VERTEX_BIT                  },
		{ "tesc",                    VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT    },
		{ "tese",                    VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT },
		{ "geom",                    VK_SHADER_STAGE_GEOMETRY_BIT                },
		{ "frag",                    VK_SHADER_STAGE_FRAGMENT_BIT                },
		{ "comp",                    VK_SHADER_STAGE_COMPUTE_BIT                 },
		{ "mesh",                    VK_SHADER_STAGE_MESH_BIT_NV                 },
		{ "rgen",                    VK_SHADER_STAGE_RAYGEN_BIT_NV               },
		{ "all",                     VK_SHADER_STAGE_ALL_GRAPHICS                },
		{ "null",                    VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM          },
		{ "spv",                     VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM          }
	};

	struct VkFormatInfo
	{
		VkFormat Format;
		uint32   Size;
	};

	// Key, VkFormat, Size.
	const std::unordered_map<std::string, VkFormatInfo> VertexAttributeMap =
	{
		{ "position",  { VK_FORMAT_R32G32B32_SFLOAT,  12u } },
		{ "color",     { VK_FORMAT_R8G8B8A8_UNORM,     4u } },
		{ "color8u",   { VK_FORMAT_R8G8B8A8_UINT,      4u } },
		{ "color32",   { VK_FORMAT_R32G32B32_SFLOAT,  12u } },
		{ "normal",    { VK_FORMAT_R32G32B32_SFLOAT,  12u } },
		{ "tangent",   { VK_FORMAT_R32G32B32_SFLOAT,  12u } },
		{ "uv",        { VK_FORMAT_R32G32_SFLOAT,      8u } }
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
		{"0",                VK_STENCIL_OP_ZERO                },
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

	const std::unordered_map<std::string, VkLogicOp> LogicOpMap =
	{
		{"clear",         VK_LOGIC_OP_CLEAR         },
		{"and",           VK_LOGIC_OP_AND           },
		{"&",             VK_LOGIC_OP_AND           },
		{"and_reverse",   VK_LOGIC_OP_AND_REVERSE   },
		{"copy",          VK_LOGIC_OP_COPY          },
		{"and_inverted",  VK_LOGIC_OP_AND_INVERTED  },
		{"no_op",         VK_LOGIC_OP_NO_OP         },
		{"xor",           VK_LOGIC_OP_XOR           },
		{"^",             VK_LOGIC_OP_XOR           },
		{"or",            VK_LOGIC_OP_OR            },
		{"|",             VK_LOGIC_OP_OR            },
		{"nor",           VK_LOGIC_OP_NOR           },
		{"equivalent",    VK_LOGIC_OP_EQUIVALENT    },
		{"==",            VK_LOGIC_OP_EQUIVALENT    },
		{"invert",        VK_LOGIC_OP_INVERT        },
		{"~",             VK_LOGIC_OP_INVERT        },
		{"or_reverse",    VK_LOGIC_OP_OR_REVERSE    },
		{"copy_inverted", VK_LOGIC_OP_COPY_INVERTED },
		{"or_inverted",   VK_LOGIC_OP_OR_INVERTED   },
		{"nand",          VK_LOGIC_OP_NAND          },
		{"set",           VK_LOGIC_OP_SET           }
	};

	const std::unordered_map<std::string, VkBlendFactor> BlendFactorMap =
	{
		{ "zero",                      VK_BLEND_FACTOR_ZERO                     },
		{ "0",                         VK_BLEND_FACTOR_ZERO                     },
		{ "one",                       VK_BLEND_FACTOR_ONE                      },
		{ "1",                         VK_BLEND_FACTOR_ONE                      },
		{ "src_color",                 VK_BLEND_FACTOR_SRC_COLOR                },
		{ "one_minus_src_color",       VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR      },
		{ "dst_color",                 VK_BLEND_FACTOR_DST_COLOR                },
		{ "one_minus_dst_color",       VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR      },
		{ "src_alpha",                 VK_BLEND_FACTOR_SRC_ALPHA                },
		{ "one_minus_src_alpha",       VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA      },
		{ "dst_alpha",                 VK_BLEND_FACTOR_DST_ALPHA                },
		{ "one_minus_dst_alpha",       VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA      },
		{ "constant_color",            VK_BLEND_FACTOR_CONSTANT_COLOR           },
		{ "one_minus_constant_color",  VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR },
		{ "constant_alpha",            VK_BLEND_FACTOR_CONSTANT_ALPHA           },
		{ "one_minus_constant_alpha",  VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA },
		{ "src_alpha_saturate",        VK_BLEND_FACTOR_SRC_ALPHA_SATURATE       },
		{ "src1_color",                VK_BLEND_FACTOR_SRC1_COLOR               },
		{ "one_minus_src1_color",      VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR     },
		{ "src1_alpha",                VK_BLEND_FACTOR_SRC1_ALPHA               },
		{ "one_minus_src1_alpha",      VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA     }
	};

	const std::unordered_map<std::string, VkBlendOp> BlendOpMap =
	{
		{ "add",                VK_BLEND_OP_ADD                    },
		{ "+",                  VK_BLEND_OP_ADD                    },
		{ "subtract",           VK_BLEND_OP_SUBTRACT               },
		{ "-",                  VK_BLEND_OP_SUBTRACT               },
		{ "reverse_subtract",   VK_BLEND_OP_REVERSE_SUBTRACT       },
		{ "min",                VK_BLEND_OP_MIN                    },
		{ "max",                VK_BLEND_OP_MAX                    },
		{ "zero",               VK_BLEND_OP_ZERO_EXT               },
		{ "src",                VK_BLEND_OP_SRC_EXT                },
		{ "dst",                VK_BLEND_OP_DST_EXT                },
		{ "src_over",           VK_BLEND_OP_SRC_OVER_EXT           },
		{ "dst_over",           VK_BLEND_OP_DST_OVER_EXT           },
		{ "src_in",             VK_BLEND_OP_SRC_IN_EXT             },
		{ "dst_in",             VK_BLEND_OP_DST_IN_EXT             },
		{ "src_out",            VK_BLEND_OP_SRC_OUT_EXT            },
		{ "dst_out",            VK_BLEND_OP_DST_OUT_EXT            },
		{ "src_atop",           VK_BLEND_OP_SRC_ATOP_EXT           },
		{ "dst_atop",           VK_BLEND_OP_DST_ATOP_EXT           },
		{ "xor",                VK_BLEND_OP_XOR_EXT                },
		{ "multiply",           VK_BLEND_OP_MULTIPLY_EXT           },
		{ "screen",             VK_BLEND_OP_SCREEN_EXT             },
		{ "overlay",            VK_BLEND_OP_OVERLAY_EXT            },
		{ "darken",             VK_BLEND_OP_DARKEN_EXT             },
		{ "lighten",            VK_BLEND_OP_LIGHTEN_EXT            },
		{ "colordodge",         VK_BLEND_OP_COLORDODGE_EXT         },
		{ "colorburn",          VK_BLEND_OP_COLORBURN_EXT          },
		{ "hardlight",          VK_BLEND_OP_HARDLIGHT_EXT          },
		{ "softlight",          VK_BLEND_OP_SOFTLIGHT_EXT          },
		{ "difference",         VK_BLEND_OP_DIFFERENCE_EXT         },
		{ "exclusion",          VK_BLEND_OP_EXCLUSION_EXT          },
		{ "invert",             VK_BLEND_OP_INVERT_EXT             },
		{ "invert_rgb",         VK_BLEND_OP_INVERT_RGB_EXT         },
		{ "lineardodge",        VK_BLEND_OP_LINEARDODGE_EXT        },
		{ "linearburn",         VK_BLEND_OP_LINEARBURN_EXT         },
		{ "vividlight",         VK_BLEND_OP_VIVIDLIGHT_EXT         },
		{ "linearlight",        VK_BLEND_OP_LINEARLIGHT_EXT        },
		{ "pinlight",           VK_BLEND_OP_PINLIGHT_EXT           },
		{ "hardmix",            VK_BLEND_OP_HARDMIX_EXT            },
		{ "hsl_hue",            VK_BLEND_OP_HSL_HUE_EXT            },
		{ "hsl_saturation",     VK_BLEND_OP_HSL_SATURATION_EXT     },
		{ "hsl_color",          VK_BLEND_OP_HSL_COLOR_EXT          },
		{ "hsl_luminosity",     VK_BLEND_OP_HSL_LUMINOSITY_EXT     },
		{ "plus",               VK_BLEND_OP_PLUS_EXT               },
		{ "plus_clamped",       VK_BLEND_OP_PLUS_CLAMPED_EXT       },
		{ "plus_clamped_alpha", VK_BLEND_OP_PLUS_CLAMPED_ALPHA_EXT },
		{ "plus_darker",        VK_BLEND_OP_PLUS_DARKER_EXT        },
		{ "minus",              VK_BLEND_OP_MINUS_EXT              },
		{ "minus_clamped",      VK_BLEND_OP_MINUS_CLAMPED_EXT      },
		{ "contrast",           VK_BLEND_OP_CONTRAST_EXT           },
		{ "invert_ovg",         VK_BLEND_OP_INVERT_OVG_EXT         },
		{ "red",                VK_BLEND_OP_RED_EXT                },
		{ "green",              VK_BLEND_OP_GREEN_EXT              },
		{ "blue",               VK_BLEND_OP_BLUE_EXT               }
	};

	const std::unordered_map<std::string, VkColorComponentFlagBits> ColorComponentMaskMap =
	{
		{ "r", VK_COLOR_COMPONENT_R_BIT },
		{ "g", VK_COLOR_COMPONENT_G_BIT },
		{ "b", VK_COLOR_COMPONENT_B_BIT },
		{ "a", VK_COLOR_COMPONENT_A_BIT }
	};

	const std::unordered_map<std::string, VkDynamicState> DynamicStateMap =
	{
		{ "viewport",                         VK_DYNAMIC_STATE_VIEWPORT                         },
		{ "scissor",                          VK_DYNAMIC_STATE_SCISSOR                          },
		{ "line_width",                       VK_DYNAMIC_STATE_LINE_WIDTH                       },
		{ "depth_bias",                       VK_DYNAMIC_STATE_DEPTH_BIAS                       },
		{ "blend_constants",                  VK_DYNAMIC_STATE_BLEND_CONSTANTS                  },
		{ "depth_bounds",                     VK_DYNAMIC_STATE_DEPTH_BOUNDS                     },
		{ "stencil_compare_mask",             VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK             },
		{ "stencil_write_mask",               VK_DYNAMIC_STATE_STENCIL_WRITE_MASK               },
		{ "stencil_reference",                VK_DYNAMIC_STATE_STENCIL_REFERENCE                },
		{ "viewport_w_scaling_nv",            VK_DYNAMIC_STATE_VIEWPORT_W_SCALING_NV            },
		{ "discard_rectangle_ext",            VK_DYNAMIC_STATE_DISCARD_RECTANGLE_EXT            },
		{ "sample_locations_ext",             VK_DYNAMIC_STATE_SAMPLE_LOCATIONS_EXT             },
		{ "viewport_shading_rate_palette_nv", VK_DYNAMIC_STATE_VIEWPORT_SHADING_RATE_PALETTE_NV },
		{ "viewport_coarse_sample_order_nv",  VK_DYNAMIC_STATE_VIEWPORT_COARSE_SAMPLE_ORDER_NV  },
		{ "exclusive_scissor_nv",             VK_DYNAMIC_STATE_EXCLUSIVE_SCISSOR_NV             },
		{ "line_stipple_ext",                 VK_DYNAMIC_STATE_LINE_STIPPLE_EXT                 },
		{ "cull_mode_ext",                    VK_DYNAMIC_STATE_CULL_MODE_EXT                    },
		{ "front_face_ext",                   VK_DYNAMIC_STATE_FRONT_FACE_EXT                   },
		{ "primitive_topology_ext",           VK_DYNAMIC_STATE_PRIMITIVE_TOPOLOGY_EXT           },
		{ "viewport_with_count_ext",          VK_DYNAMIC_STATE_VIEWPORT_WITH_COUNT_EXT          },
		{ "scissor_with_count_ext",           VK_DYNAMIC_STATE_SCISSOR_WITH_COUNT_EXT           },
		{ "vertex_input_binding_stride_ext",  VK_DYNAMIC_STATE_VERTEX_INPUT_BINDING_STRIDE_EXT  },
		{ "depth_test_enable_ext",            VK_DYNAMIC_STATE_DEPTH_TEST_ENABLE_EXT            },
		{ "depth_write_enable_ext",           VK_DYNAMIC_STATE_DEPTH_WRITE_ENABLE_EXT           },
		{ "depth_compare_op_ext",             VK_DYNAMIC_STATE_DEPTH_COMPARE_OP_EXT             },
		{ "depth_bounds_test_enable_ext",     VK_DYNAMIC_STATE_DEPTH_BOUNDS_TEST_ENABLE_EXT     },
		{ "stencil_test_enable_ext",          VK_DYNAMIC_STATE_STENCIL_TEST_ENABLE_EXT          },
		{ "state_stencil_op_ext",             VK_DYNAMIC_STATE_STENCIL_OP_EXT                   }
	};

	const char* DefaultShaderEntryPoint = "main";
	const char* DefaultPrimitiveTopology = "triangle_list";
	const char* DefaultPolygonMode = "fill";
	const char* DefaultCullMode = "cull_none";
	const char* DefaultFrontFace = "counter_clockwise";
	const char* DefaultCompareOp = "less_equal";
	const char* DefaultStencilOp = "keep";
	const char* DefaultLogicOp = "clear";
	const char* DefaultBlendFactor = "zero";
	const char* DefaultBlendOp = "add";
	const char* DefaultColorComponentMask = "rgba";
	const char* DefaultDynamicState = "viewport";
}

/// Implementation...

bool Util::GetShaderStage(const std::string& InKey, VkShaderStageFlags& OutShaderStage)
{
	try
	{
		OutShaderStage = ShaderStageMap.at(StringUtil::ToLowerCase(InKey));
		return true;
	}
	catch (const std::out_of_range& msg)
	{
		LogSystem::LogError(std::string(msg.what()) + ", pipeline stage type invalid!", _name_of(GetShaderStage));
		return false;
	}
}

VkFormat Util::GetVertexAttributeVkFormat(const std::string& InKey)
{
	VkFormat result;
	try
	{
		result = VertexAttributeMap.at(InKey).Format;
		return result;
	}
	catch (const std::out_of_range& msg)
	{
		result = VK_FORMAT_UNDEFINED;
		LogSystem::LogWarning(std::string(msg.what()) + ", Function input is not valid!", _name_of(GetVertexAttributeVkFormat));
		return result;
	}
}

uint32 Util::GetVertexAttributeSize(const std::string& InKey)
{
	uint32 result;
	try
	{
		result = VertexAttributeMap.at(InKey).Size;
		return result;
	}
	catch (const std::out_of_range& msg)
	{
		result = 0;
		LogSystem::LogWarning(std::string(msg.what()) + ", Function input is not valid!", _name_of(GetVertexAttributeSize));
		return result;
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
		LogSystem::LogWarning(std::string(msg.what()) + ", pipeline input assembly, primitive topology invalid! default set to \"" + DefaultPrimitiveTopology + "\"!", _name_of(GetPrimitiveTopology));
		return result;
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
		LogSystem::LogWarning(std::string(msg.what()) + ", pipeline rasterization state, polygon mode invalid! default set to \"" + DefaultPolygonMode + "\"!", _name_of(GetPolygonMode));
		return result;
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
		LogSystem::LogWarning(std::string(msg.what()) + ", pipeline rasterization state, cull mode invalid! default set to \"" + DefaultCullMode + "\"!", _name_of(GetCullMode));
		return result;
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
		LogSystem::LogWarning(std::string(msg.what()) + ", pipeline rasterization state, front face invalid! default set to \"" + DefaultFrontFace + "\"!", _name_of(GetFrontFace));
		return result;
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
		LogSystem::LogWarning(std::string(msg.what()) + ", pipeline depth stencil state, compare op invalid! default set to \"" + DefaultCompareOp + "\"!", _name_of(GetCompareOp));
		return result;
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
		LogSystem::LogWarning(std::string(msg.what()) + ", pipeline depth stencil state, stencil op invalid! default set to \"" + DefaultStencilOp + "\"!", _name_of(GetStencilOp));
		return result;
	}
}

VkLogicOp Util::GetLogicOp(const std::string& InKey)
{
	VkLogicOp result;
	try
	{
		result = LogicOpMap.at(InKey);
		return result;
	}
	catch (const std::out_of_range& msg)
	{
		result = VK_LOGIC_OP_CLEAR;
		LogSystem::LogWarning(std::string(msg.what()) + ", pipeline color blend state, logic op invalid! default set to \"" + DefaultLogicOp + "\"!", _name_of(GetLogicOp));
		return result;
	}
}

VkBlendFactor Util::GetBlendFactor(const std::string& InKey)
{
	VkBlendFactor result;
	try
	{
		result = BlendFactorMap.at(InKey);
		return result;
	}
	catch (const std::out_of_range& msg)
	{
		result = VK_BLEND_FACTOR_ZERO;
		LogSystem::LogWarning(std::string(msg.what()) + ", pipeline color blend state, blend factor invalid! default set to \"" + DefaultBlendFactor + "\"!", _name_of(GetBlendFactor));
		return result;
	}
}

VkBlendOp Util::GetBlendOp(const std::string& InKey)
{
	VkBlendOp result;
	try
	{
		result = BlendOpMap.at(InKey);
		return result;
	}
	catch (const std::out_of_range& msg)
	{
		result = VK_BLEND_OP_ADD;
		LogSystem::LogWarning(std::string(msg.what()) + ", pipeline color blend state, blend op invalid! default set to \"" + DefaultBlendOp + "\"!", _name_of(GetBlendOp));
		return result;
	}
}

VkColorComponentFlags Util::GetColorComponentMask(const std::string& InKey)
{
	VkColorComponentFlags result = 0;
	for (size_t i = 0; i < InKey.length(); i++)
	{
		try
		{
			result |= ColorComponentMaskMap.at(InKey.substr(i, _count_1));
		}
		catch (const std::out_of_range& msg)
		{
			LogSystem::LogWarning(std::string(msg.what()) + ", pipeline color blend state, component mask detects an invalid char present, please fill this field with \"r,g,b,a\"!", _name_of(GetColorComponentMask));
			continue;
		}
	}

	if (result == 0)
	{
		result = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		LogSystem::LogWarning("pipeline color blend state, component mask invalid! default set to \"" + std::string(DefaultColorComponentMask) + "\"!", _name_of(GetColorComponentMask));
		return result;
	}

	return result;
}

VkDynamicState Util::GetDynamicState(const std::string& InKey)
{
	VkDynamicState result;
	try
	{
		result = DynamicStateMap.at(InKey);
		return result;
	}
	catch (const std::out_of_range& msg)
	{
		result = VK_DYNAMIC_STATE_VIEWPORT;
		LogSystem::LogWarning(std::string(msg.what()) + ", pipeline dynamic state, state invalid! default set to \"" + DefaultDynamicState + "\"!", _name_of(GetDynamicState));
		return result;
	}
}

std::string Util::VkShaderStageToString(VkShaderStageFlags InStage)
{
#define VK_SHADER_STAGE_TO_STRING(stage) if (InStage == stage) return _name_of(stage);

	VK_SHADER_STAGE_TO_STRING(VK_SHADER_STAGE_VERTEX_BIT);
	VK_SHADER_STAGE_TO_STRING(VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT);
	VK_SHADER_STAGE_TO_STRING(VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT);
	VK_SHADER_STAGE_TO_STRING(VK_SHADER_STAGE_GEOMETRY_BIT);
	VK_SHADER_STAGE_TO_STRING(VK_SHADER_STAGE_FRAGMENT_BIT);
	VK_SHADER_STAGE_TO_STRING(VK_SHADER_STAGE_COMPUTE_BIT);
	VK_SHADER_STAGE_TO_STRING(VK_SHADER_STAGE_ALL_GRAPHICS);
	VK_SHADER_STAGE_TO_STRING(VK_SHADER_STAGE_ALL);
	VK_SHADER_STAGE_TO_STRING(VK_SHADER_STAGE_RAYGEN_BIT_KHR);
	VK_SHADER_STAGE_TO_STRING(VK_SHADER_STAGE_ANY_HIT_BIT_KHR);
	VK_SHADER_STAGE_TO_STRING(VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR);
	VK_SHADER_STAGE_TO_STRING(VK_SHADER_STAGE_MISS_BIT_KHR);
	VK_SHADER_STAGE_TO_STRING(VK_SHADER_STAGE_INTERSECTION_BIT_KHR);
	VK_SHADER_STAGE_TO_STRING(VK_SHADER_STAGE_CALLABLE_BIT_KHR);
	VK_SHADER_STAGE_TO_STRING(VK_SHADER_STAGE_TASK_BIT_NV);
	VK_SHADER_STAGE_TO_STRING(VK_SHADER_STAGE_MESH_BIT_NV);

	return _str_null;
}

std::string Util::VkDescriptorTypeToString(VkDescriptorType InDescType)
{
#define VK_DESCRIPTOR_TYPE_TO_STRING(type) if (InDescType == type) return _name_of(type);

	VK_DESCRIPTOR_TYPE_TO_STRING(VK_DESCRIPTOR_TYPE_SAMPLER);
    VK_DESCRIPTOR_TYPE_TO_STRING(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);
    VK_DESCRIPTOR_TYPE_TO_STRING(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE);
    VK_DESCRIPTOR_TYPE_TO_STRING(VK_DESCRIPTOR_TYPE_STORAGE_IMAGE);
    VK_DESCRIPTOR_TYPE_TO_STRING(VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER);
    VK_DESCRIPTOR_TYPE_TO_STRING(VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER);
    VK_DESCRIPTOR_TYPE_TO_STRING(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
    VK_DESCRIPTOR_TYPE_TO_STRING(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);
    VK_DESCRIPTOR_TYPE_TO_STRING(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC);
    VK_DESCRIPTOR_TYPE_TO_STRING(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC);

	return _str_null;
}