//
// StringMapper.h
//

#pragma once

#include <string>

namespace StringMapper
{
	enum class ID
	{
		Begin = 0,

		// Begin graphic_pipeline_infos.
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

		// End graphic_pipeline_infos.

		End,
		Size = End,
		Max = End,

		BEGIN_RANGE = Begin,
		END_RANGE = End,
		RANGE_SIZE = (END_RANGE - BEGIN_RANGE) + 1
	};

	extern std::string Map[size_t(ID::RANGE_SIZE)];
}