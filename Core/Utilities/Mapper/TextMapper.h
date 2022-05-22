/*********************************************************************
 *  TextMapper.h
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 * 
 *  Using specific id to index the string we need.
 *********************************************************************/

#pragma once

#include "Core/Base/BaseType.h"

class TextMapper
{

public:

	enum class ID
	{
		Begin = 0,

		/// Begin Vulkan Json Key.

		// Begin graphic_pipeline_infos.

		vk_name,
		vk_flags,
		vk_size,
		vk_offset,

		vk_graphic_pipeline_infos,

		vk_pipeline_stages_infos,
		vk_stage_type,
		vk_stage_code_path,
		vk_entrypoint,
		vk_specialization_constants,

		vk_vertex_input_attributes,
		vk_binding_id,
		vk_attributes,

		vk_pipeline_input_assembly,
		vk_primitive_topology,
		vk_primitive_restart_enable,

		vk_tessellation_state,
		vk_patch_control_points_count,

		vk_viewport_state,
		vk_viewports,
		vk_position,
		vk_depth_range,

		vk_scissor_rectangles,

		vk_rasterization_state,
		vk_depth_clamp_enable,
		vk_rasterizer_discard_enable,
		vk_polygon_mode,
		vk_cull_mode,
		vk_front_face,
		vk_depth_bias_enable,
		vk_depth_bias_constant_factor,
		vk_depth_bias_clamp,
		vk_depth_bias_slope_factor,
		vk_line_width,

		vk_multisample_state,
		vk_multisample_count,
		vk_sample_shading_enable,
		vk_min_sample_shading_factor,
		vk_sample_masks,
		vk_alpha_to_coverage_enable,
		vk_alpha_to_one_enable,

		vk_depth_stencil_state,
		vk_depth_test_enable,
		vk_depth_write_enable,
		vk_depth_compare_op,
		vk_depth_bounds_test_enable,
		vk_stencil_test_enable,
		vk_stencil_test_state,
		vk_front,
		vk_back,
		vk_fail_op,
		vk_pass_op,
		vk_depth_fail_op,
		vk_compare_op,
		vk_compare_mask,
		vk_write_mask,
		vk_reference,
		vk_min_depth_bounds,
		vk_max_depth_bounds,

		vk_color_blend_state,
		vk_logic_op_enable,
		vk_logic_op,
		vk_attachments,
		vk_blend_enable,
		vk_src_color_factor,
		vk_dst_color_factor,
		vk_color_blend_op,
		vk_src_alpha_factor,
		vk_dst_alpha_factor,
		vk_alpha_blend_op,
		vk_component_mask,
		vk_blend_constants,

		vk_dynamic_state,
		vk_state,
		vk_renderpass,
		vk_subpass,
		vk_base_pipeline,

		// End graphic_pipeline_infos.

		// Begin renderpass_info.
		
		vk_renderpass_path,
		vk_renderpass_info,
		vk_attachment_descriptions,
		vk_format,
		vk_sample_count,
		vk_load_op,
		vk_store_op,
		vk_stencil_load_op,
		vk_stencil_store_op,
		vk_in_state,
		vk_out_state,
		vk_subpass_descriptions,
		vk_pipeline_bind_point,
		vk_input_attachments,
		vk_attachment_name,
		vk_color_attachments,
		vk_resolve_attachments,
		vk_preserve_attachment_names,
		vk_depth_attachment,
		vk_subpass_dependencies,
		vk_src_subpass_name,
		vk_dst_subpass_name,
		vk_src_stage_mask,
		vk_dst_stage_mask,
		vk_src_access_mask,
		vk_dst_access_mask,
		vk_dependency_flags,

		// End renderpass_info.

		/// End Vulkan Json Key.

		End,
		Size = End,
		Max = End,

		BEGIN_RANGE = Begin,
		END_RANGE = End,
		RANGE_SIZE = (END_RANGE - BEGIN_RANGE) + 1
	};

	static string Map[usize(ID::RANGE_SIZE)];
};

#define _text_mapper(id) TextMapper::Map[(usize)TextMapper::ID::id]