//
// Utility.cpp
//

#include "Utility.h"
#include "StringManager.h"

namespace
{
	/// <summary>
	/// Utility String Map Object Tables.
	/// </summary>
	const std::unordered_map<std::string, VkShaderStageFlags> VkShaderStageMap =
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
	const std::unordered_map<std::string, VkFormatInfo> VkVertexAttributeMap =
	{
		{ "position",  { VK_FORMAT_R32G32B32_SFLOAT,  12u } },
		{ "color",     { VK_FORMAT_R8G8B8A8_UNORM,     4u } },
		{ "color8u",   { VK_FORMAT_R8G8B8A8_UINT,      4u } },
		{ "color32",   { VK_FORMAT_R32G32B32_SFLOAT,  12u } },
		{ "normal",    { VK_FORMAT_R32G32B32_SFLOAT,  12u } },
		{ "tangent",   { VK_FORMAT_R32G32B32_SFLOAT,  12u } },
		{ "uv",        { VK_FORMAT_R32G32_SFLOAT,      8u } }
	};

	const std::unordered_map<std::string, VkPrimitiveTopology> VkPrimitiveTopologyMap =
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

	const std::unordered_map<std::string, VkPolygonMode> VkPolygonModeMap =
	{
		{ "fill",   VK_POLYGON_MODE_FILL  },
		{ "line",   VK_POLYGON_MODE_LINE  },
		{ "point",  VK_POLYGON_MODE_POINT }
	};

	const std::unordered_map<std::string, VkCullModeFlags> VkCullModeFlagsMap =
	{
		{ "cull_none",   VK_CULL_MODE_NONE           },
		{ "cull_front",  VK_CULL_MODE_FRONT_BIT      },
		{ "cull_back",   VK_CULL_MODE_BACK_BIT       },
		{ "cull_both",   VK_CULL_MODE_FRONT_AND_BACK }
	};

	const std::unordered_map<std::string, VkFrontFace> VkFrontFaceMap =
	{
		{ "counter_clockwise",  VK_FRONT_FACE_COUNTER_CLOCKWISE },
		{ "clockwise",          VK_FRONT_FACE_CLOCKWISE         }
	};

	const std::unordered_map<std::string, VkCompareOp> VkCompareOpMap =
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

	const std::unordered_map<std::string, VkStencilOp> VkStencilOpMap =
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

	const std::unordered_map<std::string, VkLogicOp> VkLogicOpMap =
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

	const std::unordered_map<std::string, VkBlendFactor> VkBlendFactorMap =
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

	const std::unordered_map<std::string, VkBlendOp> VkBlendOpMap =
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

	const std::unordered_map<std::string, VkColorComponentFlags> VkColorComponentMaskMap =
	{
		{ "r", VK_COLOR_COMPONENT_R_BIT },
		{ "g", VK_COLOR_COMPONENT_G_BIT },
		{ "b", VK_COLOR_COMPONENT_B_BIT },
		{ "a", VK_COLOR_COMPONENT_A_BIT }
	};

	const std::unordered_map<std::string, VkDynamicState> VkDynamicStateMap =
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

	const std::unordered_map<std::string, VkFormat> VkFormatMap =
	{
		{ "UNDEFINED",                                       VK_FORMAT_UNDEFINED                                      },
		{ "R4G4_UNORM_PACK8",                                VK_FORMAT_R4G4_UNORM_PACK8                               },
		{ "R4G4B4A4_UNORM_PACK16",                           VK_FORMAT_R4G4B4A4_UNORM_PACK16                          },
		{ "B4G4R4A4_UNORM_PACK16",                           VK_FORMAT_B4G4R4A4_UNORM_PACK16                          },
		{ "R5G6B5_UNORM_PACK16",                             VK_FORMAT_R5G6B5_UNORM_PACK16                            },
		{ "B5G6R5_UNORM_PACK16",                             VK_FORMAT_B5G6R5_UNORM_PACK16                            },
		{ "R5G5B5A1_UNORM_PACK16",                           VK_FORMAT_R5G5B5A1_UNORM_PACK16                          },
		{ "B5G5R5A1_UNORM_PACK16",                           VK_FORMAT_B5G5R5A1_UNORM_PACK16                          },
		{ "A1R5G5B5_UNORM_PACK16",                           VK_FORMAT_A1R5G5B5_UNORM_PACK16                          },
		{ "R8_UNORM",                                        VK_FORMAT_R8_UNORM                                       },
		{ "R8_SNORM",                                        VK_FORMAT_R8_SNORM                                       },
		{ "R8_USCALED",                                      VK_FORMAT_R8_USCALED                                     },
		{ "R8_SSCALED",                                      VK_FORMAT_R8_SSCALED                                     },
		{ "R8_UINT",                                         VK_FORMAT_R8_UINT                                        },
		{ "R8_SINT",                                         VK_FORMAT_R8_SINT                                        },
		{ "R8_SRGB",                                         VK_FORMAT_R8_SRGB                                        },
		{ "R8G8_UNORM",                                      VK_FORMAT_R8G8_UNORM                                     },
		{ "R8G8_SNORM",                                      VK_FORMAT_R8G8_SNORM                                     },
		{ "R8G8_USCALED",                                    VK_FORMAT_R8G8_USCALED                                   },
		{ "R8G8_SSCALED",                                    VK_FORMAT_R8G8_SSCALED                                   },
		{ "R8G8_UINT",                                       VK_FORMAT_R8G8_UINT                                      },
		{ "R8G8_SINT",                                       VK_FORMAT_R8G8_SINT                                      },
		{ "R8G8_SRGB",                                       VK_FORMAT_R8G8_SRGB                                      },
		{ "R8G8B8_UNORM",                                    VK_FORMAT_R8G8B8_UNORM                                   },
		{ "R8G8B8_SNORM",                                    VK_FORMAT_R8G8B8_SNORM                                   },
		{ "R8G8B8_USCALED",                                  VK_FORMAT_R8G8B8_USCALED                                 },
		{ "R8G8B8_SSCALED",                                  VK_FORMAT_R8G8B8_SSCALED                                 },
		{ "R8G8B8_UINT",                                     VK_FORMAT_R8G8B8_UINT                                    },
		{ "R8G8B8_SINT",                                     VK_FORMAT_R8G8B8_SINT                                    },
		{ "R8G8B8_SRGB",                                     VK_FORMAT_R8G8B8_SRGB                                    },
		{ "B8G8R8_UNORM",                                    VK_FORMAT_B8G8R8_UNORM                                   },
		{ "B8G8R8_SNORM",                                    VK_FORMAT_B8G8R8_SNORM                                   },
		{ "B8G8R8_USCALED",                                  VK_FORMAT_B8G8R8_USCALED                                 },
		{ "B8G8R8_SSCALED",                                  VK_FORMAT_B8G8R8_SSCALED                                 },
		{ "B8G8R8_UINT",                                     VK_FORMAT_B8G8R8_UINT                                    },
		{ "B8G8R8_SINT",                                     VK_FORMAT_B8G8R8_SINT                                    },
		{ "B8G8R8_SRGB",                                     VK_FORMAT_B8G8R8_SRGB                                    },
		{ "R8G8B8A8_UNORM",                                  VK_FORMAT_R8G8B8A8_UNORM                                 },
		{ "R8G8B8A8_SNORM",                                  VK_FORMAT_R8G8B8A8_SNORM                                 },
		{ "R8G8B8A8_USCALED",                                VK_FORMAT_R8G8B8A8_USCALED                               },
		{ "R8G8B8A8_SSCALED",                                VK_FORMAT_R8G8B8A8_SSCALED                               },
		{ "R8G8B8A8_UINT",                                   VK_FORMAT_R8G8B8A8_UINT                                  },
		{ "R8G8B8A8_SINT",                                   VK_FORMAT_R8G8B8A8_SINT                                  },
		{ "R8G8B8A8_SRGB",                                   VK_FORMAT_R8G8B8A8_SRGB                                  },
		{ "B8G8R8A8_UNORM",                                  VK_FORMAT_B8G8R8A8_UNORM                                 },
		{ "B8G8R8A8_SNORM",                                  VK_FORMAT_B8G8R8A8_SNORM                                 },
		{ "B8G8R8A8_USCALED",                                VK_FORMAT_B8G8R8A8_USCALED                               },
		{ "B8G8R8A8_SSCALED",                                VK_FORMAT_B8G8R8A8_SSCALED                               },
		{ "B8G8R8A8_UINT",                                   VK_FORMAT_B8G8R8A8_UINT                                  },
		{ "B8G8R8A8_SINT",                                   VK_FORMAT_B8G8R8A8_SINT                                  },
		{ "B8G8R8A8_SRGB",                                   VK_FORMAT_B8G8R8A8_SRGB                                  },
		{ "A8B8G8R8_UNORM_PACK32",                           VK_FORMAT_A8B8G8R8_UNORM_PACK32                          },
		{ "A8B8G8R8_SNORM_PACK32",                           VK_FORMAT_A8B8G8R8_SNORM_PACK32                          },
		{ "A8B8G8R8_USCALED_PACK32",                         VK_FORMAT_A8B8G8R8_USCALED_PACK32                        },
		{ "A8B8G8R8_SSCALED_PACK32",                         VK_FORMAT_A8B8G8R8_SSCALED_PACK32                        },
		{ "A8B8G8R8_UINT_PACK32",                            VK_FORMAT_A8B8G8R8_UINT_PACK32                           },
		{ "A8B8G8R8_SINT_PACK32",                            VK_FORMAT_A8B8G8R8_SINT_PACK32                           },
		{ "A8B8G8R8_SRGB_PACK32",                            VK_FORMAT_A8B8G8R8_SRGB_PACK32                           },
		{ "A2R10G10B10_UNORM_PACK32",                        VK_FORMAT_A2R10G10B10_UNORM_PACK32                       },
		{ "A2R10G10B10_SNORM_PACK32",                        VK_FORMAT_A2R10G10B10_SNORM_PACK32                       },
		{ "A2R10G10B10_USCALED_PACK32",                      VK_FORMAT_A2R10G10B10_USCALED_PACK32                     },
		{ "A2R10G10B10_SSCALED_PACK32",                      VK_FORMAT_A2R10G10B10_SSCALED_PACK32                     },
		{ "A2R10G10B10_UINT_PACK32",                         VK_FORMAT_A2R10G10B10_UINT_PACK32                        },
		{ "A2R10G10B10_SINT_PACK32",                         VK_FORMAT_A2R10G10B10_SINT_PACK32                        },
		{ "A2B10G10R10_UNORM_PACK32",                        VK_FORMAT_A2B10G10R10_UNORM_PACK32                       },
		{ "A2B10G10R10_SNORM_PACK32",                        VK_FORMAT_A2B10G10R10_SNORM_PACK32                       },
		{ "A2B10G10R10_USCALED_PACK32",                      VK_FORMAT_A2B10G10R10_USCALED_PACK32                     },
		{ "A2B10G10R10_SSCALED_PACK32",                      VK_FORMAT_A2B10G10R10_SSCALED_PACK32                     },
		{ "A2B10G10R10_UINT_PACK32",                         VK_FORMAT_A2B10G10R10_UINT_PACK32                        },
		{ "A2B10G10R10_SINT_PACK32",                         VK_FORMAT_A2B10G10R10_SINT_PACK32                        },
		{ "R16_UNORM",                                       VK_FORMAT_R16_UNORM                                      },
		{ "R16_SNORM",                                       VK_FORMAT_R16_SNORM                                      },
		{ "R16_USCALED",                                     VK_FORMAT_R16_USCALED                                    },
		{ "R16_SSCALED",                                     VK_FORMAT_R16_SSCALED                                    },
		{ "R16_UINT",                                        VK_FORMAT_R16_UINT                                       },
		{ "R16_SINT",                                        VK_FORMAT_R16_SINT                                       },
		{ "R16_SFLOAT",                                      VK_FORMAT_R16_SFLOAT                                     },
		{ "R16G16_UNORM",                                    VK_FORMAT_R16G16_UNORM                                   },
		{ "R16G16_SNORM",                                    VK_FORMAT_R16G16_SNORM                                   },
		{ "R16G16_USCALED",                                  VK_FORMAT_R16G16_USCALED                                 },
		{ "R16G16_SSCALED",                                  VK_FORMAT_R16G16_SSCALED                                 },
		{ "R16G16_UINT",                                     VK_FORMAT_R16G16_UINT                                    },
		{ "R16G16_SINT",                                     VK_FORMAT_R16G16_SINT                                    },
		{ "R16G16_SFLOAT",                                   VK_FORMAT_R16G16_SFLOAT                                  },
		{ "R16G16B16_UNORM",                                 VK_FORMAT_R16G16B16_UNORM                                },
		{ "R16G16B16_SNORM",                                 VK_FORMAT_R16G16B16_SNORM                                },
		{ "R16G16B16_USCALED",                               VK_FORMAT_R16G16B16_USCALED                              },
		{ "R16G16B16_SSCALED",                               VK_FORMAT_R16G16B16_SSCALED                              },
		{ "R16G16B16_UINT",                                  VK_FORMAT_R16G16B16_UINT                                 },
		{ "R16G16B16_SINT",                                  VK_FORMAT_R16G16B16_SINT                                 },
		{ "R16G16B16_SFLOAT",                                VK_FORMAT_R16G16B16_SFLOAT                               },
		{ "R16G16B16A16_UNORM",                              VK_FORMAT_R16G16B16A16_UNORM                             },
		{ "R16G16B16A16_SNORM",                              VK_FORMAT_R16G16B16A16_SNORM                             },
		{ "R16G16B16A16_USCALED",                            VK_FORMAT_R16G16B16A16_USCALED                           },
		{ "R16G16B16A16_SSCALED",                            VK_FORMAT_R16G16B16A16_SSCALED                           },
		{ "R16G16B16A16_UINT",                               VK_FORMAT_R16G16B16A16_UINT                              },
		{ "R16G16B16A16_SINT",                               VK_FORMAT_R16G16B16A16_SINT                              },
		{ "R16G16B16A16_SFLOAT",                             VK_FORMAT_R16G16B16A16_SFLOAT                            },
		{ "R32_UINT",                                        VK_FORMAT_R32_UINT                                       },
		{ "R32_SINT",                                        VK_FORMAT_R32_SINT                                       },
		{ "R32_SFLOAT",                                      VK_FORMAT_R32_SFLOAT                                     },
		{ "R32G32_UINT",                                     VK_FORMAT_R32G32_UINT                                    },
		{ "R32G32_SINT",                                     VK_FORMAT_R32G32_SINT                                    },
		{ "R32G32_SFLOAT",                                   VK_FORMAT_R32G32_SFLOAT                                  },
		{ "R32G32B32_UINT",                                  VK_FORMAT_R32G32B32_UINT                                 },
		{ "R32G32B32_SINT",                                  VK_FORMAT_R32G32B32_SINT                                 },
		{ "R32G32B32_SFLOAT",                                VK_FORMAT_R32G32B32_SFLOAT                               },
		{ "R32G32B32A32_UINT",                               VK_FORMAT_R32G32B32A32_UINT                              },
		{ "R32G32B32A32_SINT",                               VK_FORMAT_R32G32B32A32_SINT                              },
		{ "R32G32B32A32_SFLOAT",                             VK_FORMAT_R32G32B32A32_SFLOAT                            },
		{ "R64_UINT",                                        VK_FORMAT_R64_UINT                                       },
		{ "R64_SINT",                                        VK_FORMAT_R64_SINT                                       },
		{ "R64_SFLOAT",                                      VK_FORMAT_R64_SFLOAT                                     },
		{ "R64G64_UINT",                                     VK_FORMAT_R64G64_UINT                                    },
		{ "R64G64_SINT",                                     VK_FORMAT_R64G64_SINT                                    },
		{ "R64G64_SFLOAT",                                   VK_FORMAT_R64G64_SFLOAT                                  },
		{ "R64G64B64_UINT",                                  VK_FORMAT_R64G64B64_UINT                                 },
		{ "R64G64B64_SINT",                                  VK_FORMAT_R64G64B64_SINT                                 },
		{ "R64G64B64_SFLOAT",                                VK_FORMAT_R64G64B64_SFLOAT                               },
		{ "R64G64B64A64_UINT",                               VK_FORMAT_R64G64B64A64_UINT                              },
		{ "R64G64B64A64_SINT",                               VK_FORMAT_R64G64B64A64_SINT                              },
		{ "R64G64B64A64_SFLOAT",                             VK_FORMAT_R64G64B64A64_SFLOAT                            },
		{ "B10G11R11_UFLOAT_PACK32",                         VK_FORMAT_B10G11R11_UFLOAT_PACK32                        },
		{ "E5B9G9R9_UFLOAT_PACK32",                          VK_FORMAT_E5B9G9R9_UFLOAT_PACK32                         },
		{ "D16_UNORM",                                       VK_FORMAT_D16_UNORM                                      },
		{ "X8_D24_UNORM_PACK32",                             VK_FORMAT_X8_D24_UNORM_PACK32                            },
		{ "D32_SFLOAT",                                      VK_FORMAT_D32_SFLOAT                                     },
		{ "S8_UINT",                                         VK_FORMAT_S8_UINT                                        },
		{ "D16_UNORM_S8_UINT",                               VK_FORMAT_D16_UNORM_S8_UINT                              },
		{ "D24_UNORM_S8_UINT",                               VK_FORMAT_D24_UNORM_S8_UINT                              },
		{ "D32_SFLOAT_S8_UINT",                              VK_FORMAT_D32_SFLOAT_S8_UINT                             },
		{ "BC1_RGB_UNORM_BLOCK",                             VK_FORMAT_BC1_RGB_UNORM_BLOCK                            },
		{ "BC1_RGB_SRGB_BLOCK",                              VK_FORMAT_BC1_RGB_SRGB_BLOCK                             },
		{ "BC1_RGBA_UNORM_BLOCK",                            VK_FORMAT_BC1_RGBA_UNORM_BLOCK                           },
		{ "BC1_RGBA_SRGB_BLOCK",                             VK_FORMAT_BC1_RGBA_SRGB_BLOCK                            },
		{ "BC2_UNORM_BLOCK",                                 VK_FORMAT_BC2_UNORM_BLOCK                                },
		{ "BC2_SRGB_BLOCK",                                  VK_FORMAT_BC2_SRGB_BLOCK                                 },
		{ "BC3_UNORM_BLOCK",                                 VK_FORMAT_BC3_UNORM_BLOCK                                },
		{ "BC3_SRGB_BLOCK",                                  VK_FORMAT_BC3_SRGB_BLOCK                                 },
		{ "BC4_UNORM_BLOCK",                                 VK_FORMAT_BC4_UNORM_BLOCK                                },
		{ "BC4_SNORM_BLOCK",                                 VK_FORMAT_BC4_SNORM_BLOCK                                },
		{ "BC5_UNORM_BLOCK",                                 VK_FORMAT_BC5_UNORM_BLOCK                                },
		{ "BC5_SNORM_BLOCK",                                 VK_FORMAT_BC5_SNORM_BLOCK                                },
		{ "BC6H_UFLOAT_BLOCK",                               VK_FORMAT_BC6H_UFLOAT_BLOCK                              },
		{ "BC6H_SFLOAT_BLOCK",                               VK_FORMAT_BC6H_SFLOAT_BLOCK                              },
		{ "BC7_UNORM_BLOCK",                                 VK_FORMAT_BC7_UNORM_BLOCK                                },
		{ "BC7_SRGB_BLOCK",                                  VK_FORMAT_BC7_SRGB_BLOCK                                 },
		{ "ETC2_R8G8B8_UNORM_BLOCK",                         VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK                        },
		{ "ETC2_R8G8B8_SRGB_BLOCK",                          VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK                         },
		{ "ETC2_R8G8B8A1_UNORM_BLOCK",                       VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK                      },
		{ "ETC2_R8G8B8A1_SRGB_BLOCK",                        VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK                       },
		{ "ETC2_R8G8B8A8_UNORM_BLOCK",                       VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK                      },
		{ "ETC2_R8G8B8A8_SRGB_BLOCK",                        VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK                       },
		{ "EAC_R11_UNORM_BLOCK",                             VK_FORMAT_EAC_R11_UNORM_BLOCK                            },
		{ "EAC_R11_SNORM_BLOCK",                             VK_FORMAT_EAC_R11_SNORM_BLOCK                            },
		{ "EAC_R11G11_UNORM_BLOCK",                          VK_FORMAT_EAC_R11G11_UNORM_BLOCK                         },
		{ "EAC_R11G11_SNORM_BLOCK",                          VK_FORMAT_EAC_R11G11_SNORM_BLOCK                         },
		{ "ASTC_4x4_UNORM_BLOCK",                            VK_FORMAT_ASTC_4x4_UNORM_BLOCK                           },
		{ "ASTC_4x4_SRGB_BLOCK",                             VK_FORMAT_ASTC_4x4_SRGB_BLOCK                            },
		{ "ASTC_5x4_UNORM_BLOCK",                            VK_FORMAT_ASTC_5x4_UNORM_BLOCK                           },
		{ "ASTC_5x4_SRGB_BLOCK",                             VK_FORMAT_ASTC_5x4_SRGB_BLOCK                            },
		{ "ASTC_5x5_UNORM_BLOCK",                            VK_FORMAT_ASTC_5x5_UNORM_BLOCK                           },
		{ "ASTC_5x5_SRGB_BLOCK",                             VK_FORMAT_ASTC_5x5_SRGB_BLOCK                            },
		{ "ASTC_6x5_UNORM_BLOCK",                            VK_FORMAT_ASTC_6x5_UNORM_BLOCK                           },
		{ "ASTC_6x5_SRGB_BLOCK",                             VK_FORMAT_ASTC_6x5_SRGB_BLOCK                            },
		{ "ASTC_6x6_UNORM_BLOCK",                            VK_FORMAT_ASTC_6x6_UNORM_BLOCK                           },
		{ "ASTC_6x6_SRGB_BLOCK",                             VK_FORMAT_ASTC_6x6_SRGB_BLOCK                            },
		{ "ASTC_8x5_UNORM_BLOCK",                            VK_FORMAT_ASTC_8x5_UNORM_BLOCK                           },
		{ "ASTC_8x5_SRGB_BLOCK",                             VK_FORMAT_ASTC_8x5_SRGB_BLOCK                            },
		{ "ASTC_8x6_UNORM_BLOCK",                            VK_FORMAT_ASTC_8x6_UNORM_BLOCK                           },
		{ "ASTC_8x6_SRGB_BLOCK",                             VK_FORMAT_ASTC_8x6_SRGB_BLOCK                            },
		{ "ASTC_8x8_UNORM_BLOCK",                            VK_FORMAT_ASTC_8x8_UNORM_BLOCK                           },
		{ "ASTC_8x8_SRGB_BLOCK",                             VK_FORMAT_ASTC_8x8_SRGB_BLOCK                            },
		{ "ASTC_10x5_UNORM_BLOCK",                           VK_FORMAT_ASTC_10x5_UNORM_BLOCK                          },
		{ "ASTC_10x5_SRGB_BLOCK",                            VK_FORMAT_ASTC_10x5_SRGB_BLOCK                           },
		{ "ASTC_10x6_UNORM_BLOCK",                           VK_FORMAT_ASTC_10x6_UNORM_BLOCK                          },
		{ "ASTC_10x6_SRGB_BLOCK",                            VK_FORMAT_ASTC_10x6_SRGB_BLOCK                           },
		{ "ASTC_10x8_UNORM_BLOCK",                           VK_FORMAT_ASTC_10x8_UNORM_BLOCK                          },
		{ "ASTC_10x8_SRGB_BLOCK",                            VK_FORMAT_ASTC_10x8_SRGB_BLOCK                           },
		{ "ASTC_10x10_UNORM_BLOCK",                          VK_FORMAT_ASTC_10x10_UNORM_BLOCK                         },
		{ "ASTC_10x10_SRGB_BLOCK",                           VK_FORMAT_ASTC_10x10_SRGB_BLOCK                          },
		{ "ASTC_12x10_UNORM_BLOCK",                          VK_FORMAT_ASTC_12x10_UNORM_BLOCK                         },
		{ "ASTC_12x10_SRGB_BLOCK",                           VK_FORMAT_ASTC_12x10_SRGB_BLOCK                          },
		{ "ASTC_12x12_UNORM_BLOCK",                          VK_FORMAT_ASTC_12x12_UNORM_BLOCK                         },
		{ "ASTC_12x12_SRGB_BLOCK",                           VK_FORMAT_ASTC_12x12_SRGB_BLOCK                          },
		{ "G8B8G8R8_422_UNORM",                              VK_FORMAT_G8B8G8R8_422_UNORM                             },
		{ "B8G8R8G8_422_UNORM",                              VK_FORMAT_B8G8R8G8_422_UNORM                             },
		{ "G8_B8_R8_3PLANE_420_UNORM",                       VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM                      },
		{ "G8_B8R8_2PLANE_420_UNORM",                        VK_FORMAT_G8_B8R8_2PLANE_420_UNORM                       },
		{ "G8_B8_R8_3PLANE_422_UNORM",                       VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM                      },
		{ "G8_B8R8_2PLANE_422_UNORM",                        VK_FORMAT_G8_B8R8_2PLANE_422_UNORM                       },
		{ "G8_B8_R8_3PLANE_444_UNORM",                       VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM                      },
		{ "R10X6_UNORM_PACK16",                              VK_FORMAT_R10X6_UNORM_PACK16                             },
		{ "R10X6G10X6_UNORM_2PACK16",                        VK_FORMAT_R10X6G10X6_UNORM_2PACK16                       },
		{ "R10X6G10X6B10X6A10X6_UNORM_4PACK16",              VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16             },
		{ "G10X6B10X6G10X6R10X6_422_UNORM_4PACK16",          VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16         },
		{ "B10X6G10X6R10X6G10X6_422_UNORM_4PACK16",          VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16         },
		{ "G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16",      VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16     },
		{ "G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16",       VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16      },
		{ "G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16",      VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16     },
		{ "G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16",       VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16      },
		{ "G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16",      VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16     },
		{ "R12X4_UNORM_PACK16",                              VK_FORMAT_R12X4_UNORM_PACK16                             },
		{ "R12X4G12X4_UNORM_2PACK16",                        VK_FORMAT_R12X4G12X4_UNORM_2PACK16                       },
		{ "R12X4G12X4B12X4A12X4_UNORM_4PACK16",              VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16             },
		{ "G12X4B12X4G12X4R12X4_422_UNORM_4PACK16",          VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16         },
		{ "B12X4G12X4R12X4G12X4_422_UNORM_4PACK16",          VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16         },
		{ "G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16",      VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16     },
		{ "G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16",       VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16      },
		{ "G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16",      VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16     },
		{ "G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16",       VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16      },
		{ "G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16",      VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16     },
		{ "G16B16G16R16_422_UNORM",                          VK_FORMAT_G16B16G16R16_422_UNORM                         },
		{ "B16G16R16G16_422_UNORM",                          VK_FORMAT_B16G16R16G16_422_UNORM                         },
		{ "G16_B16_R16_3PLANE_420_UNORM",                    VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM                   },
		{ "G16_B16R16_2PLANE_420_UNORM",                     VK_FORMAT_G16_B16R16_2PLANE_420_UNORM                    },
		{ "G16_B16_R16_3PLANE_422_UNORM",                    VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM                   },
		{ "G16_B16R16_2PLANE_422_UNORM",                     VK_FORMAT_G16_B16R16_2PLANE_422_UNORM                    },
		{ "G16_B16_R16_3PLANE_444_UNORM",                    VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM                   },
		{ "PVRTC1_2BPP_UNORM_BLOCK_IMG",                     VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG                    },
		{ "PVRTC1_4BPP_UNORM_BLOCK_IMG",                     VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG                    },
		{ "PVRTC2_2BPP_UNORM_BLOCK_IMG",                     VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG                    },
		{ "PVRTC2_4BPP_UNORM_BLOCK_IMG",                     VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG                    },
		{ "PVRTC1_2BPP_SRGB_BLOCK_IMG",                      VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG                     },
		{ "PVRTC1_4BPP_SRGB_BLOCK_IMG",                      VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG                     },
		{ "PVRTC2_2BPP_SRGB_BLOCK_IMG",                      VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG                     },
		{ "PVRTC2_4BPP_SRGB_BLOCK_IMG",                      VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG                     },
		{ "ASTC_4x4_SFLOAT_BLOCK_EXT",                       VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK_EXT                      },
		{ "ASTC_5x4_SFLOAT_BLOCK_EXT",                       VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK_EXT                      },
		{ "ASTC_5x5_SFLOAT_BLOCK_EXT",                       VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK_EXT                      },
		{ "ASTC_6x5_SFLOAT_BLOCK_EXT",                       VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK_EXT                      },
		{ "ASTC_6x6_SFLOAT_BLOCK_EXT",                       VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK_EXT                      },
		{ "ASTC_8x5_SFLOAT_BLOCK_EXT",                       VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK_EXT                      },
		{ "ASTC_8x6_SFLOAT_BLOCK_EXT",                       VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK_EXT                      },
		{ "ASTC_8x8_SFLOAT_BLOCK_EXT",                       VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK_EXT                      },
		{ "ASTC_10x5_SFLOAT_BLOCK_EXT",                      VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK_EXT                     },
		{ "ASTC_10x6_SFLOAT_BLOCK_EXT",                      VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK_EXT                     },
		{ "ASTC_10x8_SFLOAT_BLOCK_EXT",                      VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK_EXT                     },
		{ "ASTC_10x10_SFLOAT_BLOCK_EXT",                     VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK_EXT                    },
		{ "ASTC_12x10_SFLOAT_BLOCK_EXT",                     VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK_EXT                    },
		{ "ASTC_12x12_SFLOAT_BLOCK_EXT",                     VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK_EXT                    },
		{ "G8B8G8R8_422_UNORM_KHR",                          VK_FORMAT_G8B8G8R8_422_UNORM_KHR                         },
		{ "B8G8R8G8_422_UNORM_KHR",                          VK_FORMAT_B8G8R8G8_422_UNORM_KHR                         },
		{ "G8_B8_R8_3PLANE_420_UNORM_KHR",                   VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM_KHR                  },
		{ "G8_B8R8_2PLANE_420_UNORM_KHR",                    VK_FORMAT_G8_B8R8_2PLANE_420_UNORM_KHR                   },
		{ "G8_B8_R8_3PLANE_422_UNORM_KHR",                   VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM_KHR                  },
		{ "G8_B8R8_2PLANE_422_UNORM_KHR",                    VK_FORMAT_G8_B8R8_2PLANE_422_UNORM_KHR                   },
		{ "G8_B8_R8_3PLANE_444_UNORM_KHR",                   VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM_KHR                  },
		{ "R10X6_UNORM_PACK16_KHR",                          VK_FORMAT_R10X6_UNORM_PACK16_KHR                         },
		{ "R10X6G10X6_UNORM_2PACK16_KHR",                    VK_FORMAT_R10X6G10X6_UNORM_2PACK16_KHR                   },
		{ "R10X6G10X6B10X6A10X6_UNORM_4PACK16_KHR",          VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16_KHR         },
		{ "G10X6B10X6G10X6R10X6_422_UNORM_4PACK16_KHR",      VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16_KHR     },
		{ "B10X6G10X6R10X6G10X6_422_UNORM_4PACK16_KHR",      VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16_KHR     },
		{ "G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16_KHR",  VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16_KHR },
		{ "G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16_KHR",   VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16_KHR  },
		{ "G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16_KHR",  VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16_KHR },
		{ "G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16_KHR",   VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16_KHR  },
		{ "G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16_KHR",  VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16_KHR },
		{ "R12X4_UNORM_PACK16_KHR",                          VK_FORMAT_R12X4_UNORM_PACK16_KHR                         },
		{ "R12X4G12X4_UNORM_2PACK16_KHR",                    VK_FORMAT_R12X4G12X4_UNORM_2PACK16_KHR                   },
		{ "R12X4G12X4B12X4A12X4_UNORM_4PACK16_KHR",          VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16_KHR         },
		{ "G12X4B12X4G12X4R12X4_422_UNORM_4PACK16_KHR",      VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16_KHR     },
		{ "B12X4G12X4R12X4G12X4_422_UNORM_4PACK16_KHR",      VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16_KHR     },
		{ "G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16_KHR",  VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16_KHR },
		{ "G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16_KHR",   VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16_KHR  },
		{ "G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16_KHR",  VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16_KHR },
		{ "G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16_KHR",   VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16_KHR  },
		{ "G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16_KHR",  VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16_KHR },
		{ "G16B16G16R16_422_UNORM_KHR",                      VK_FORMAT_G16B16G16R16_422_UNORM_KHR                     },
		{ "B16G16R16G16_422_UNORM_KHR",                      VK_FORMAT_B16G16R16G16_422_UNORM_KHR                     },
		{ "G16_B16_R16_3PLANE_420_UNORM_KHR",                VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM_KHR               },
		{ "G16_B16R16_2PLANE_420_UNORM_KHR",                 VK_FORMAT_G16_B16R16_2PLANE_420_UNORM_KHR                },
		{ "G16_B16_R16_3PLANE_422_UNORM_KHR",                VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM_KHR               },
		{ "G16_B16R16_2PLANE_422_UNORM_KHR",                 VK_FORMAT_G16_B16R16_2PLANE_422_UNORM_KHR                },
		{ "G16_B16_R16_3PLANE_444_UNORM_KHR",                VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM_KHR               }
	};

	const std::unordered_map<std::string, VkAttachmentLoadOp> VkAttachmentLoadOpMap =
	{
		{ "load",       VK_ATTACHMENT_LOAD_OP_LOAD      },
		{ "clear",      VK_ATTACHMENT_LOAD_OP_CLEAR     },
		{ "dont_care",  VK_ATTACHMENT_LOAD_OP_DONT_CARE }
	};

	const std::unordered_map<std::string, VkAttachmentStoreOp> VkAttachmentStoreOpMap =
	{
		{ "store",      VK_ATTACHMENT_STORE_OP_STORE     },
		{ "dont_care",  VK_ATTACHMENT_STORE_OP_DONT_CARE }
	};

	const std::unordered_map<std::string, VkImageLayout> VkImageLayoutMap =
	{
		{ "UNDEFINED",                                       VK_IMAGE_LAYOUT_UNDEFINED                                      },
		{ "GENERAL",                                         VK_IMAGE_LAYOUT_GENERAL                                        },
		{ "COLOR_ATTACHMENT_OPTIMAL",                        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL                       },
		{ "DEPTH_STENCIL_ATTACHMENT_OPTIMAL",                VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL               },
		{ "DEPTH_STENCIL_READ_ONLY_OPTIMAL",                 VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL                },
		{ "SHADER_READ_ONLY_OPTIMAL",                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL                       },
		{ "TRANSFER_SRC_OPTIMAL",                            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL                           },
		{ "TRANSFER_DST_OPTIMAL",                            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL                           },
		{ "PREINITIALIZED",                                  VK_IMAGE_LAYOUT_PREINITIALIZED                                 },
		{ "DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL",      VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL     },
		{ "DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL",      VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL     },
		{ "DEPTH_ATTACHMENT_OPTIMAL",                        VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL                       },
		{ "DEPTH_READ_ONLY_OPTIMAL",                         VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL                        },
		{ "STENCIL_ATTACHMENT_OPTIMAL",                      VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL                     },
		{ "STENCIL_READ_ONLY_OPTIMAL",                       VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL                      },
		{ "PRESENT_SRC_KHR",                                 VK_IMAGE_LAYOUT_PRESENT_SRC_KHR                                },
		{ "SHARED_PRESENT_KHR",                              VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR                             },
		{ "SHADING_RATE_OPTIMAL_NV",                         VK_IMAGE_LAYOUT_SHADING_RATE_OPTIMAL_NV                        },
		{ "FRAGMENT_DENSITY_MAP_OPTIMAL_EXT",                VK_IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT               },
		{ "DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL_KHR",  VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL_KHR },
		{ "DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL_KHR",  VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL_KHR },
		{ "DEPTH_ATTACHMENT_OPTIMAL_KHR",                    VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL_KHR                   },
		{ "DEPTH_READ_ONLY_OPTIMAL_KHR",                     VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL_KHR                    },
		{ "STENCIL_ATTACHMENT_OPTIMAL_KHR",                  VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL_KHR                 },
		{ "STENCIL_READ_ONLY_OPTIMAL_KHR",                   VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL_KHR                  },

		{ "undefined",                                       VK_IMAGE_LAYOUT_UNDEFINED                                      },
		{ "general",                                         VK_IMAGE_LAYOUT_GENERAL                                        },
		{ "color_write",                                     VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL                       },
		{ "depth_stencil_write",                             VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL               },
		{ "depth_stencil_readonly",                          VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL                },
		{ "shader_readonly",                                 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL                       },
		{ "transfer_src",                                    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL                           },
		{ "transfer_dst",                                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL                           },
		{ "preinitialized",                                  VK_IMAGE_LAYOUT_PREINITIALIZED                                 },
		{ "depth_readonly_stencil_write",                    VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL     },
		{ "depth_write_stencil_readonly",                    VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL     },
		{ "depth_write",                                     VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL                       },
		{ "depth_readonly",                                  VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL                        },
		{ "stencil_write",                                   VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL                     },
		{ "stencil_readonly",                                VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL                      },
		{ "present",                                         VK_IMAGE_LAYOUT_PRESENT_SRC_KHR                                }
	};

	const std::unordered_map<std::string, VkPipelineBindPoint> VkPipelineBindPointMap =
	{
		{ "graphics",     VK_PIPELINE_BIND_POINT_GRAPHICS        },
		{ "compute",      VK_PIPELINE_BIND_POINT_COMPUTE         },
		{ "ray_tracing",  VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR }
	};

	const std::unordered_map<std::string, VkPipelineStageFlags> VkPipelineStageFlagsMap =
	{
		{ "top",                           VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT                      },
		{ "draw_indirect",                 VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT                    },
		{ "vertex_input",                  VK_PIPELINE_STAGE_VERTEX_INPUT_BIT                     },
		{ "vertex",                        VK_PIPELINE_STAGE_VERTEX_SHADER_BIT                    },
		{ "tessellation_control",          VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT      },
		{ "tessellation_evaluation",       VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT   },
		{ "geometry",                      VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT                  },
		{ "fragment",                      VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT                  },
		{ "early_fragment",                VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT             },
		{ "late_fragment",                 VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT              },
		{ "color_output",                  VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT          },
		{ "compute",                       VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT                   },
		{ "transfer",                      VK_PIPELINE_STAGE_TRANSFER_BIT                         },
		{ "bottom",                        VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT                   },
		{ "host",                          VK_PIPELINE_STAGE_HOST_BIT                             },
		{ "all_graphics",                  VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT                     },
		{ "all_commands",                  VK_PIPELINE_STAGE_ALL_COMMANDS_BIT                     },
		{ "transform_feedback",            VK_PIPELINE_STAGE_TRANSFORM_FEEDBACK_BIT_EXT           },
		{ "conditional_rendering",         VK_PIPELINE_STAGE_CONDITIONAL_RENDERING_BIT_EXT        },
		{ "ray_tracing",                   VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_KHR           },
		{ "acceleration_structure_build",  VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR },
		{ "shading_rate_image",            VK_PIPELINE_STAGE_SHADING_RATE_IMAGE_BIT_NV            },
		{ "task_shader",                   VK_PIPELINE_STAGE_TASK_SHADER_BIT_NV                   },
		{ "mesh_shader",                   VK_PIPELINE_STAGE_MESH_SHADER_BIT_NV                   },
		{ "fragment_density_process",      VK_PIPELINE_STAGE_FRAGMENT_DENSITY_PROCESS_BIT_EXT     },
		{ "command_preprocess",            VK_PIPELINE_STAGE_COMMAND_PREPROCESS_BIT_NV            }
	};

	const std::unordered_map<std::string, VkAccessFlags> VkAccessFlagsMap =
	{
		{ "indirect_command_read",              VK_ACCESS_INDIRECT_COMMAND_READ_BIT                 },
		{ "index_read",                         VK_ACCESS_INDEX_READ_BIT                            },
		{ "vertex_attribute_read",              VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT                 },
		{ "uniform_read",                       VK_ACCESS_UNIFORM_READ_BIT                          },
		{ "input_attachment_read",              VK_ACCESS_INPUT_ATTACHMENT_READ_BIT                 },
		{ "shader_read",                        VK_ACCESS_SHADER_READ_BIT                           },
		{ "shader_write",                       VK_ACCESS_SHADER_WRITE_BIT                          },
		{ "color_attachment_read",              VK_ACCESS_COLOR_ATTACHMENT_READ_BIT                 },
		{ "color_attachment_write",             VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT                },
		{ "depth_stencil_attachment_read",      VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT         },
		{ "depth_stencil_attachment_write",     VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT        },
		{ "transfer_read",                      VK_ACCESS_TRANSFER_READ_BIT                         },
		{ "transfer_write",                     VK_ACCESS_TRANSFER_WRITE_BIT                        },
		{ "host_read",                          VK_ACCESS_HOST_READ_BIT                             },
		{ "host_write",                         VK_ACCESS_HOST_WRITE_BIT                            },
		{ "memory_read",                        VK_ACCESS_MEMORY_READ_BIT                           },
		{ "memory_write",                       VK_ACCESS_MEMORY_WRITE_BIT                          },
		{ "transform_feedback_write",           VK_ACCESS_TRANSFORM_FEEDBACK_WRITE_BIT_EXT          },
		{ "transform_feedback_counter_read",    VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT   },
		{ "transform_feedback_counter_write",   VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT  },
		{ "conditional_rendering_read",         VK_ACCESS_CONDITIONAL_RENDERING_READ_BIT_EXT        },
		{ "color_attachment_read_noncoherent",  VK_ACCESS_COLOR_ATTACHMENT_READ_NONCOHERENT_BIT_EXT },
		{ "acceleration_structure_read",        VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR       },
		{ "acceleration_structure_write",       VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR      },
		{ "shading_rate_image_read",            VK_ACCESS_SHADING_RATE_IMAGE_READ_BIT_NV            },
		{ "fragment_density_map_read",          VK_ACCESS_FRAGMENT_DENSITY_MAP_READ_BIT_EXT         },
		{ "command_preprocess_read",            VK_ACCESS_COMMAND_PREPROCESS_READ_BIT_NV            },
		{ "command_preprocess_write",           VK_ACCESS_COMMAND_PREPROCESS_WRITE_BIT_NV           }
	};

	const std::unordered_map<std::string, VkDependencyFlags> VkDependencyFlagsMap =
	{
		{ "region",        VK_DEPENDENCY_BY_REGION_BIT        },
		{ "device_group",  VK_DEPENDENCY_DEVICE_GROUP_BIT     },
		{ "view_local",    VK_DEPENDENCY_VIEW_LOCAL_BIT       }
	};

	const char* DefaultShaderEntryPoint     = "main";
	const char* DefaultVkPrimitiveTopology  = "triangle_list";
	const char* DefaultVkPolygonMode        = "fill";
	const char* DefaultVkCullModeFlags      = "cull_none";
	const char* DefaultVkFrontFace          = "counter_clockwise";
	const char* DefaultVkCompareOp          = "less_equal";
	const char* DefaultVkStencilOp          = "keep";
	const char* DefaultVkLogicOp            = "clear";
	const char* DefaultVkBlendFactor        = "zero";
	const char* DefaultVkBlendOp            = "add";
	const char* DefaultColorComponentMask   = "rgba";
	const char* DefaultVkDynamicState       = "viewport";
	const char* DefaultVkFormat             = "R8G8B8A8_UNORM";
	const char* DefaultVkAttachmentLoadOp   = "dont_care";
	const char* DefaultVkAttachmentStoreOp  = "dont_care";
	const char* DefaultVkImageLayout        = "undefined";
	const char* DefaultVkPipelineBindPoint  = "graphics";
	const char* DefaultVkPipelineStageFlags = "all_commands";
	const char* DefaultVkAccessFlags        = "memory_read";
	const char* DefaultVkDependencyFlags    = "region";
}

/// Implementation...

bool Util::GetShaderStage(const std::string& InKey, VkShaderStageFlags& OutShaderStage)
{
	try
	{
		OutShaderStage = VkShaderStageMap.at(StringUtil::ToLowerCase(InKey));
		return true;
	}
	catch (const std::out_of_range& msg)
	{
		_log_error(std::string(msg.what()) + ", pipeline stage type invalid!", _name_of(GetShaderStage));
		return false;
	}
}

VkFormat Util::GetVertexAttributeVkFormat(const std::string& InKey)
{
	VkFormat result;
	try
	{
		result = VkVertexAttributeMap.at(InKey).Format;
		return result;
	}
	catch (const std::out_of_range& msg)
	{
		result = VK_FORMAT_UNDEFINED;
		_log_warning(std::string(msg.what()) + ", Function input is not valid!", _name_of(GetVertexAttributeVkFormat));
		return result;
	}
}

uint32 Util::GetVertexAttributeSize(const std::string& InKey)
{
	uint32 result;
	try
	{
		result = VkVertexAttributeMap.at(InKey).Size;
		return result;
	}
	catch (const std::out_of_range& msg)
	{
		result = 0;
		_log_warning(std::string(msg.what()) + ", Function input is not valid!", _name_of(GetVertexAttributeSize));
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
			result |= VkColorComponentMaskMap.at(InKey.substr(i, _count_1));
		}
		catch (const std::out_of_range& msg)
		{
			_log_warning(std::string(msg.what()) + ", pipeline color blend state, component mask detects an invalid char present, please fill this field with \"r,g,b,a\"!", _name_of(GetColorComponentMask));
			continue;
		}
	}

	if (result == 0)
	{
		result = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		_log_warning("pipeline color blend state, component mask invalid! default set to \"" + std::string(DefaultColorComponentMask) + "\"!", _name_of(GetColorComponentMask));
		return result;
	}

	return result;
}

VkSampleCountFlags Util::GetMultisampleCount(uint32 InCount)
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

/*--------------------------------------------------------------------
                              boundary
---------------------------------------------------------------------*/

#define GET_VK_TYPE_IMPL(type)                                        \
type Util::Get##type(const std::string& InKey)                        \
{                                                                     \
	type result;                                                      \
	try                                                               \
	{                                                                 \
		result = type##Map.at(InKey);                                 \
		return result;                                                \
	}                                                                 \
	catch (const std::out_of_range& msg)                              \
	{                                                                 \
		result = type##Map.at(Default##type);                         \
		_log_warning(std::string(msg.what()) +                        \
			"Map failed because of invalid key! default set to \"" +  \
			Default##type + "\"!", _name_of(Get##type));              \
		return result;                                                \
	}                                                                 \
}                                                                     \


GET_VK_TYPE_IMPL(VkPrimitiveTopology)
GET_VK_TYPE_IMPL(VkPolygonMode)
GET_VK_TYPE_IMPL(VkCullModeFlags)
GET_VK_TYPE_IMPL(VkFrontFace)
GET_VK_TYPE_IMPL(VkCompareOp)
GET_VK_TYPE_IMPL(VkStencilOp)
GET_VK_TYPE_IMPL(VkLogicOp)
GET_VK_TYPE_IMPL(VkBlendFactor)
GET_VK_TYPE_IMPL(VkBlendOp)
GET_VK_TYPE_IMPL(VkDynamicState)
GET_VK_TYPE_IMPL(VkFormat)
GET_VK_TYPE_IMPL(VkAttachmentLoadOp)
GET_VK_TYPE_IMPL(VkAttachmentStoreOp)
GET_VK_TYPE_IMPL(VkImageLayout)
GET_VK_TYPE_IMPL(VkPipelineBindPoint)
GET_VK_TYPE_IMPL(VkPipelineStageFlags)
GET_VK_TYPE_IMPL(VkAccessFlags)
GET_VK_TYPE_IMPL(VkDependencyFlags)


/*--------------------------------------------------------------------
							  boundary
---------------------------------------------------------------------*/

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
