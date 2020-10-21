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
		size,
		offset,

		graphic_pipeline_infos,
		pipeline_flags,

		pipeline_stages_infos,
		stage_flags,
		stage_type,
		stage_code_path,
		entrypoint,
		specialization_constants,

		vertex_input_attributes,
		binding_id,
		attributes,

		pipeline_input_assembly,
		primitive_topology,
		primitive_restart_enable,

		tessellation_state,
		patch_control_points_count,

		viewport_state,
		viewports,
		position,
		depth_range,

		scissor_rectangles,

		rasterization_state,
		depth_clamp_enable,
		rasterizer_discard_enable,
		polygon_mode,
		cull_mode,
		front_face,
		depth_bias_enable,
		depth_bias_constant_factor,
		depth_bias_clamp,
		depth_bias_slope_factor,
		line_width,

		multisample_state,
		multisample_count,
		sample_shading_enable,
		min_sample_shading_factor,
		sample_masks,
		alpha_to_coverage_enable,
		alpha_to_one_enable,

		depth_stencil_state,
		depth_test_enable,
		depth_write_enable,
		depth_compare_op,
		depth_bounds_test_enable,
		stencil_test_enable,
		stencil_test_state,
		front,
		back,
		fail_op,
		pass_op,
		depth_fail_op,
		compare_op,
		compare_mask,
		write_mask,
		reference,
		min_depth_bounds,
		max_depth_bounds,

		color_blend_state,
		logic_op_enable,
		logic_op,
		attachments,
		blend_enable,
		src_color_factor,
		dst_color_factor,
		color_blend_op,
		src_alpha_factor,
		dst_alpha_factor,
		alpha_blend_op,
		component_mask,
		blend_constants,

		dynamic_state,
		state,

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