/*********************************************************************
 *  GLSLCompiler.cpp
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 *********************************************************************/
#include "GLSLCompiler.h"
#include "Core/Utility/String/StringManager.h"
#include "Core/Utility/Loader/ModuleLoader.h"
#include "Core/Utility/EnumToString.h"
#include "Core/Utility/Utility.h"

_impl_create_interface(GLSLCompiler)

GLSLCompiler::GLSLCompiler()
{
	m_pModule = ModuleLoader::Create(this);

    if (m_pModule->Load(Path(StringUtil::Printf("Tools/CMBuild/GLSLCompiler/%/GLSLCompiler.dll", _platform))))
    {
        m_pInterface = m_pModule->GetInterface<PFGetGLSLCompilerInterface>("GetInterface");

        if (m_pInterface != nullptr)
        {
            m_pCompiler = m_pInterface();
            if (m_pCompiler != nullptr) m_pCompiler->Init();
        }
    }
}

GLSLCompiler::~GLSLCompiler()
{
    if (m_pCompiler != nullptr)
    {
        for (auto& pSPVData : m_pSPVData)
        {
            if (pSPVData != nullptr)
                m_pCompiler->Free(pSPVData);
        }

        m_pCompiler->Close();
    }
}

void GLSLCompiler::CompileShader(VkShaderStageFlags InStageType, const Path& InShaderPath, const CompileInfo* InCompileInfo)
{
    if (m_pCompiler != nullptr)
    {
        m_pSPVData.push_back(m_pCompiler->CompileFromPath(InStageType, InShaderPath.ToCString(), InCompileInfo));
    }
    else
    {
        _log_error("The GLSLCompiler has not been Init!", LogSystem::Category::GLSLCompiler);
        return;
    }
}

bool GLSLCompiler::CheckAndParseSPVData(uint32 InMaxDescSets, VkPushConstantRange& OutPushConstantRange, std::vector<std::vector<VkDescriptorSetLayoutBinding>>& OutDescSets)
{
	std::vector<std::vector<uint8>> descSetsBindingID;

	uint8 maxSetID = 0u;
	std::vector<uint8> setIDArray;

	for (auto& spvData : m_pSPVData)
	{
		for (uint32 descType = VK_DESCRIPTOR_TYPE_SAMPLER; descType < GLSLCompiler::NumDescriptorType; descType++)
		{
			for (uint32 i = 0; i < spvData->resource[descType].count; i++)
			{
				uint8& currentSet = spvData->resource[descType].items[i].set;

				maxSetID = std::max<uint8>(maxSetID, currentSet);

				if (!Utility::IsVecContain<uint8>(setIDArray, currentSet, _lambda_is_equal(uint8)))
					setIDArray.push_back(currentSet);
			}
		}
	}

	// Sort the setIDArray using operator <.
	std::sort(setIDArray.begin(), setIDArray.end());
	// Check the continuity of setID.
	for (uint8 i = 0u; i < setIDArray.size() - 1u; i++)
	{
		if ((setIDArray[i] + 1u) != setIDArray[i + 1u])
		{
			_log_error("The creating pipeline has discontinuous DescriptorSet ID!", LogSystem::Category::GLSLCompiler);
			return false;
		}
	}

	uint8 numSets = std::min<uint8>(maxSetID + 1u, InMaxDescSets);

	descSetsBindingID.resize(numSets);
	OutDescSets.resize(numSets);

	for (auto& spvData : m_pSPVData)
	{
		GLSLCompiler::ShaderResourceData resData = spvData->resource[GLSLCompiler::ResID_PushConstant];

		static bool bPushConstantInit = false;
		if (!bPushConstantInit && resData.count > 0u)
		{
			OutPushConstantRange.stageFlags = spvData->shader_stage;
			OutPushConstantRange.offset = _offset_0;
			OutPushConstantRange.size = resData.items[_index_0].size;

			if (resData.count > 1u) _log_warning("The creating pipeline has too many push constant blocks!", LogSystem::Category::GLSLCompiler);

			bPushConstantInit = true; // There can only be one push constant block.
		}

		for (uint32 descType = VK_DESCRIPTOR_TYPE_SAMPLER; descType < VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT; descType++)
		{
			for (uint32 i = 0; i < spvData->resource[descType].count; i++)
			{
				VkDescriptorSetLayoutBinding descSetBinding;
				descSetBinding.binding = spvData->resource[descType].items[i].binding;
				descSetBinding.descriptorType = (VkDescriptorType)descType;
				descSetBinding.stageFlags = spvData->shader_stage;
				descSetBinding.descriptorCount = 1u;
				descSetBinding.pImmutableSamplers = nullptr;

				uint8& bindingID = spvData->resource[descType].items[i].binding;
				auto& currentSetBindingID = descSetsBindingID[spvData->resource[descType].items[i].set];

				if (Utility::IsVecContain<uint8>(currentSetBindingID, bindingID, _lambda_is_equal(uint8)))
				{
					_log_error(StringUtil::Printf("The creating pipeline has repeated binding ID in its shader, the stage is %, the variable name is %!",
						EnumToString::VkShaderStageToString(descSetBinding.stageFlags), spvData->resource[descType].items[i].name), LogSystem::Category::GLSLCompiler);
					return false;
				}

				currentSetBindingID.push_back(bindingID);
				OutDescSets[spvData->resource[descType].items[i].set].push_back(descSetBinding);
			}
		}
	}

	// It Recommended that Binding ID in a Set is continuous from 0.
	for (uint8 i = 0; i < descSetsBindingID.size(); i++)
	{
		auto& bindingIDArray = descSetsBindingID[i];
		// Sort the bindingIDArray using operator <.
		std::sort(bindingIDArray.begin(), bindingIDArray.end());
		std::sort(OutDescSets[i].begin(), OutDescSets[i].end(), [&](const VkDescriptorSetLayoutBinding& left, const VkDescriptorSetLayoutBinding& right) { return left.binding < right.binding; });
		// Check the continuity of bindingID.
		for (uint8 j = 0u; j < bindingIDArray.size(); j++)
		{
			if (bindingIDArray[j] != j)
			{
				_log_warning(StringUtil::Printf("The creating pipeline has discontinuous binding ID in its shader, the stage is %, the descriptor type is %, %",
					EnumToString::VkShaderStageToString(OutDescSets[i][j].stageFlags), EnumToString::VkDescriptorTypeToString(OutDescSets[i][j].descriptorType),
					"it's recommended that you don't create sparsely populated sets because this can waste resources in the device!"), 
					LogSystem::Category::GLSLCompiler);
			}
		}
	}

	return true;
}

void GLSLCompiler::FlushSPVData()
{
    if (m_pCompiler != nullptr)
    {
        for (auto& pSPVData : m_pSPVData)
        {
            if (pSPVData != nullptr)
                m_pCompiler->Free(pSPVData);
        }

        m_pSPVData.clear();
    }
}

bool GLSLCompiler::HasValidSPVData()
{
    return !m_pSPVData.empty() && m_pSPVData[_index_0] != nullptr;
}

GLSLCompiler::SPVData* GLSLCompiler::GetLastSPVData()
{
    return m_pSPVData.empty() ? nullptr : m_pSPVData[m_pSPVData.size() - 1];
}

std::vector<GLSLCompiler::SPVData*>& GLSLCompiler::GetAllSPVData()
{
    return m_pSPVData;
}
