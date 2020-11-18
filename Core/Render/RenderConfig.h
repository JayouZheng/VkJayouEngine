//
// RenderConfig.h
//

#pragma once

#include "Core/Common.h"

namespace GConfig
{
	namespace Sampler
	{
		static float SamplerMaxLod = 16.0f;
		static float MaxAnisotropy = 8.0f;
	}

	namespace Subresource
	{
		const VkImageSubresourceRange ColorSubResRange =
		{
			VK_IMAGE_ASPECT_COLOR_BIT,   // aspectMask
			0,                           // baseMipLevel
			VK_REMAINING_MIP_LEVELS,     // levelCount
			0,                           // baseArrayLayer
			VK_REMAINING_ARRAY_LAYERS    // layerCount
		};

		const VkImageSubresourceRange DepthStencilSubResRange =
		{
			VK_IMAGE_ASPECT_DEPTH_BIT |
			VK_IMAGE_ASPECT_STENCIL_BIT, // aspectMask
			0,                           // baseMipLevel
			VK_REMAINING_MIP_LEVELS,     // levelCount
			0,                           // baseArrayLayer
			VK_REMAINING_ARRAY_LAYERS    // layerCount
		};
	}

	namespace Pipeline
	{
		static const VkPipelineInputAssemblyStateCreateInfo DefaultInputAssemblyStateInfo =
		{
			VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO, // sType
			nullptr,                                                     // pNext
			_flag_none,                                                  // flags
			VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,                         // topology
			VK_FALSE                                                     // primitiveRestartEnable
		};

		static const VkPipelineTessellationStateCreateInfo  DefaultTessellationStateInfo =
		{
			VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,   // sType
			nullptr,                                                     // pNext
			_flag_none,                                                  // flags
			0u                                                           // patchControlPoints
		};

		static const VkViewport                             DefaultViewport =
		{
			0.0f,                                                        // x
			0.0f,                                                        // y
			1280.0f,                                                     // width
			720.0f,                                                      // height
			0.0f,                                                        // minDepth
			1.0f                                                         // maxDepth
		};

		static const VkRect2D                               DefaultScissor =
		{
			{ 0, 0 },                                                    // offset
			{ 1280u, 720u }                                              // extent
		};

		static const VkPipelineViewportStateCreateInfo      DefaultViewportStateInfo =
		{
			VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,       // sType
			nullptr,                                                     // pNext
			_flag_none,                                                  // flags
			_count_1,                                                    // viewportCount
			&DefaultViewport,                                            // pViewports
			_count_1,                                                    // scissorCount
			&DefaultScissor                                              // pScissors
		};

		static const VkPipelineRasterizationStateCreateInfo DefaultRasterizationStateInfo =
		{
			VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,  // sType
			nullptr,                                                     // pNext
			_flag_none,                                                  // flags
			VK_FALSE,                                                    // depthClampEnable
			VK_TRUE,                                                     // rasterizerDiscardEnable
			VK_POLYGON_MODE_FILL,                                        // polygonMode
			VK_CULL_MODE_NONE,                                           // cullMode
			VK_FRONT_FACE_COUNTER_CLOCKWISE,                             // frontFace
			VK_FALSE,                                                    // depthBiasEnable
			0.0f,                                                        // depthBiasConstantFactor
			0.0f,                                                        // depthBiasClamp
			0.0f,                                                        // depthBiasSlopeFactor
			0.0f                                                         // lineWidth
		};

		static const VkPipelineMultisampleStateCreateInfo   DefaultMultisampleStateInfo =
		{
			VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,   // sType
			nullptr,												    // pNext
			_flag_none,												    // flags
			VK_SAMPLE_COUNT_1_BIT,									    // rasterizationSamples
			VK_FALSE,												    // sampleShadingEnable
			0.0f,													    // minSampleShading
			nullptr,												    // pSampleMask
			VK_FALSE,												    // alphaToCoverageEnable
			VK_FALSE												    // alphaToOneEnable
		};

		static const VkStencilOpState                       DefaultStencilOpState =
		{
			VK_STENCIL_OP_KEEP,										    // failOp
			VK_STENCIL_OP_KEEP,										    // passOp
			VK_STENCIL_OP_KEEP,										    // depthFailOp
			VK_COMPARE_OP_ALWAYS,									    // compareOp
			0x00,													    // compareMask
			0x00,													    // writeMask
			0u														    // reference
		};

		static const VkPipelineDepthStencilStateCreateInfo  DefaultDepthStencilStateInfo =
		{
			VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO, // sType
			nullptr,													// pNext
			_flag_none,													// flags
			VK_TRUE,													// depthTestEnable
			VK_TRUE,													// depthWriteEnable
			VK_COMPARE_OP_LESS_OR_EQUAL,								// depthCompareOp
			VK_FALSE,													// depthBoundsTestEnable
			VK_FALSE,													// stencilTestEnable
			DefaultStencilOpState,										// front
			DefaultStencilOpState,										// back
			0.0f,														// minDepthBounds
			0.0f														// maxDepthBounds
		};

		static const VkPipelineColorBlendAttachmentState    DefaultColorBlendAttachmentState =
		{
			VK_TRUE,                                                    // blendEnable
			VK_BLEND_FACTOR_SRC_ALPHA,                                  // srcColorBlendFactor
			VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,                        // dstColorBlendFactor
			VK_BLEND_OP_ADD,                                            // colorBlendOp
			VK_BLEND_FACTOR_ONE,                                        // srcAlphaBlendFactor
			VK_BLEND_FACTOR_ZERO,                                       // dstAlphaBlendFactor
			VK_BLEND_OP_ADD,                                            // alphaBlendOp
			VK_COLOR_COMPONENT_R_BIT |                                  // colorWriteMask
			VK_COLOR_COMPONENT_G_BIT |
			VK_COLOR_COMPONENT_B_BIT |
			VK_COLOR_COMPONENT_A_BIT
		};

		static const VkPipelineColorBlendStateCreateInfo    DefaultColorBlendStateInfo =
		{
			VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,   // sType
			nullptr,                                                    // pNext
			_flag_none,                                                 // flags
			VK_FALSE,                                                   // logicOpEnable
			VK_LOGIC_OP_CLEAR,                                          // logicOp
			_count_1,                                                   // attachmentCount
			&DefaultColorBlendAttachmentState,                          // pAttachments
			{ 0.0f, 0.0f, 0.0f, 0.0f }                                  // blendConstants[4]
		};

		static const std::array<VkDynamicState, 2>          DefaultVkDynamicState =
		{
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		static const VkPipelineDynamicStateCreateInfo       DefaultDynamicStateInfo =
		{
			VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,       // sType
			nullptr,                                                    // pNext
			_flag_none,                                                 // flags
			static_cast<uint32>(DefaultVkDynamicState.size()),          // dynamicStateCount
			DefaultVkDynamicState.data(),                               // pDynamicStates
		};
	}
}