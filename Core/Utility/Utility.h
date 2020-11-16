//
// Utility.h
//

#pragma once

#include "Core/Common.h"

namespace Util
{
	bool                     GetShaderStage(const std::string& InKey, VkShaderStageFlags& OutShaderStage);
	VkFormat                 GetVertexAttributeVkFormat(const std::string& InKey);
	uint32                   GetVertexAttributeSize(const std::string& InKey);
	VkSampleCountFlags       GetMultisampleCount(uint32 InCount);
	VkColorComponentFlags    GetColorComponentMask(const std::string& InKey);

	VkPrimitiveTopology      GetVkPrimitiveTopology(const std::string& InKey);
	VkPolygonMode            GetVkPolygonMode(const std::string& InKey);
	VkCullModeFlags          GetVkCullModeFlags(const std::string& InKey);
	VkFrontFace              GetVkFrontFace(const std::string& InKey);
	VkCompareOp              GetVkCompareOp(const std::string& InKey);
	VkStencilOp              GetVkStencilOp(const std::string& InKey);
	VkLogicOp                GetVkLogicOp(const std::string& InKey);
	VkBlendFactor            GetVkBlendFactor(const std::string& InKey);
	VkBlendOp                GetVkBlendOp(const std::string& InKey);	
	VkDynamicState           GetVkDynamicState(const std::string& InKey);
	VkFormat                 GetVkFormat(const std::string& InKey);
	VkAttachmentLoadOp       GetVkAttachmentLoadOp(const std::string& InKey);
	VkAttachmentStoreOp      GetVkAttachmentStoreOp(const std::string& InKey);
	VkImageLayout            GetVkImageLayout(const std::string& InKey);
	VkPipelineBindPoint      GetVkPipelineBindPoint(const std::string& InKey);
	VkPipelineStageFlags     GetVkPipelineStageFlags(const std::string& InKey);
	VkAccessFlags            GetVkAccessFlags(const std::string& InKey);
	VkDependencyFlags        GetVkDependencyFlags(const std::string& InKey);

	std::string              VkShaderStageToString(VkShaderStageFlags InStage);
	std::string              VkDescriptorTypeToString(VkDescriptorType InDescType);

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