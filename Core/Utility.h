//
// Utility.h
//

#pragma once

#include "Common.h"

namespace Util
{
	bool                     GetShaderStage(const std::string& InKey, VkShaderStageFlags& OutShaderStage);
	VkFormat                 GetVertexAttributeVkFormat(const std::string& InKey);
	uint32                   GetVertexAttributeSize(const std::string& InKey);
	VkPrimitiveTopology      GetPrimitiveTopology(const std::string& InKey);
	VkPolygonMode            GetPolygonMode(const std::string& InKey);
	VkCullModeFlagBits       GetCullMode(const std::string& InKey);
	VkFrontFace              GetFrontFace(const std::string& InKey);
	VkSampleCountFlagBits    GetMultisampleCount(uint32             InCount);
	VkCompareOp              GetCompareOp(const std::string& InKey);
	VkStencilOp              GetStencilOp(const std::string& InKey);
	VkLogicOp                GetLogicOp(const std::string& InKey);
	VkBlendFactor            GetBlendFactor(const std::string& InKey);
	VkBlendOp                GetBlendOp(const std::string& InKey);
	VkColorComponentFlags    GetColorComponentMask(const std::string& InKey);
	VkDynamicState           GetDynamicState(const std::string& InKey);

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

	template<typename T>
	bool IsVkGuaranteedMinimum(T InValue, T InMinimum)
	{
		return InValue >= InMinimum;
	}
}