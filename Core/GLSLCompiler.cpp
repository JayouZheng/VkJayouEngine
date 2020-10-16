﻿//
// GLSLCompiler.cpp
//

#include "GLSLCompiler.h"
#include "DiskResourceLoader.h"
#include "StringManager.h"

GLSLCompiler::GLSLCompiler()
{
    m_module.Load(DiskResourceLoader::Load(StringUtil::Printf("ThirdParty/CMBuild/GLSLCompiler/%/GLSLCompiler.dll", _platform)));

    m_pInterface = m_module.GetInterface<PFGetGLSLCompilerInterface>("GetInterface");

    if (m_pInterface != nullptr)
    {
        m_pCompiler = m_pInterface();
        if (m_pCompiler != nullptr) m_pCompiler->Init();
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

    m_module.Free();
}

void GLSLCompiler::CompileShader(VkShaderStageFlags InStageType, const std::string& InShaderPath, const CompileInfo* InCompileInfo)
{
    if (m_pCompiler != nullptr)
    {
        m_pSPVData.push_back(m_pCompiler->CompileFromPath(InStageType, InShaderPath.data(), InCompileInfo));
    }
    else _return_log("The GLSLCompiler has not been Init!");
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