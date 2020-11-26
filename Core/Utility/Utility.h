/*********************************************************************
 *  Utility.h
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 * 
 *  .
 *********************************************************************/

#pragma once

#include "Core/Common.h"

namespace Util
{
	bool                     GetShaderStage(const string& InKey, VkShaderStageFlags& OutShaderStage);
	VkFormat                 GetVertexAttributeVkFormat(const string& InKey);
	uint32                   GetVertexAttributeSize(const string& InKey);
	VkSampleCountFlags       GetMultisampleCount(uint32 InCount);
	VkColorComponentFlags    GetColorComponentMask(const string& InKey);

	VkPrimitiveTopology      GetVkPrimitiveTopology(const string& InKey);
	VkPolygonMode            GetVkPolygonMode(const string& InKey);
	VkCullModeFlags          GetVkCullModeFlags(const string& InKey);
	VkFrontFace              GetVkFrontFace(const string& InKey);
	VkCompareOp              GetVkCompareOp(const string& InKey);
	VkStencilOp              GetVkStencilOp(const string& InKey);
	VkLogicOp                GetVkLogicOp(const string& InKey);
	VkBlendFactor            GetVkBlendFactor(const string& InKey);
	VkBlendOp                GetVkBlendOp(const string& InKey);	
	VkDynamicState           GetVkDynamicState(const string& InKey);
	VkFormat                 GetVkFormat(const string& InKey);
	VkAttachmentLoadOp       GetVkAttachmentLoadOp(const string& InKey);
	VkAttachmentStoreOp      GetVkAttachmentStoreOp(const string& InKey);
	VkImageLayout            GetVkImageLayout(const string& InKey);
	VkPipelineBindPoint      GetVkPipelineBindPoint(const string& InKey);
	VkPipelineStageFlags     GetVkPipelineStageFlags(const string& InKey);
	VkAccessFlags            GetVkAccessFlags(const string& InKey);
	VkDependencyFlags        GetVkDependencyFlags(const string& InKey);

	string                   VkShaderStageToString(VkShaderStageFlags InStage);
	string                   VkDescriptorTypeToString(VkDescriptorType InDescType);

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
}