﻿/*********************************************************************
 *  EnumToString.cpp
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 *********************************************************************/

#include "EnumToString.h"

string EnumToString::VkShaderStageToString(VkShaderStageFlags InStage)
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

string EnumToString::VkDescriptorTypeToString(VkDescriptorType InDescType)
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