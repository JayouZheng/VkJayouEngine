//
// GLSLCompiler.cpp
//

#include "GLSLCompiler.h"
#include "DiskResourceLoader.h"
#include "StringManager.h"
#include "ModuleLoader.h"

GLSLCompiler::GLSLCompiler()
{
    m_pModule = new ModuleLoader;

    if (m_pModule->Load(DiskResourceLoader::Load(StringUtil::Printf("ThirdParty/CMBuild/GLSLCompiler/%/GLSLCompiler.dll", _platform))))
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

    m_pModule->Free();

    delete m_pModule;
}

void GLSLCompiler::CompileShader(VkShaderStageFlags InStageType, const std::string& InShaderPath, const CompileInfo* InCompileInfo)
{
    if (m_pCompiler != nullptr)
    {
        m_pSPVData.push_back(m_pCompiler->CompileFromPath(InStageType, InShaderPath.data(), InCompileInfo));
    }
    else
    {
        LogSystem::LogError("The GLSLCompiler has not been Init!", LogSystem::Category::GLSLCompiler);
        return;
    }
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
