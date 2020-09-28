//
// GLSLCompiler.h
//

#pragma once

#include "ModuleLoader.h"

constexpr uint32 NumDescriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT + 1u;

class GLSLCompiler
{

public:

    struct ShaderStage
    {
        char   name[128];
        uint8  location;
        uint32 basetype;
        uint32 vec_size;
    };

    struct ShaderStageData
    {
        uint32 count;
        ShaderStage* items;
    };

    struct ShaderResource
    {
        char name[128];

        uint8 set;
        uint8 binding;
    };

    struct ShaderResourceData
    {
        uint32 count;
        ShaderResource* items;
    };

    struct SPVData
    {
        bool  result;
        char* log;
        char* debug_log;

        uint32* spv_data;
        uint32  spv_length;

        ShaderStageData input, output;
        ShaderResourceData resource[NumDescriptorType];
    };

    struct GLSLCompilerInterface
    {
        bool        (*Init)    ();
        void        (*Close)   ();

        uint32      (*GetType) (const char* ext_name);
        SPVData*    (*Compile) (const uint32 type, const char* source);

        void        (*Free)    (SPVData*);
    };

    typedef GLSLCompilerInterface* (*PFGetGLSLCompilerInterface)();


    GLSLCompiler()
    {
        m_module.Load(Global::GetModulePath() + "ThirdParty/CMBuild/GLSLCompiler/GLSLCompiler.dll");

        m_pInterface = m_module.GetInterface<PFGetGLSLCompilerInterface>("GetInterface");

        if (m_pInterface != nullptr)
        {
            m_pCompiler = m_pInterface();
            if (m_pCompiler != nullptr) m_pCompiler->Init();
        }
    }

    ~GLSLCompiler()
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

    void FlushSPVData()
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

    void CompileShader(VkShaderStageFlags InStageType, const std::string& InShaderPath)
    {
        if (m_pCompiler != nullptr)
        {
            std::ifstream is(InShaderPath, std::ios::binary | std::ios::in | std::ios::ate);

            if (is.is_open())
            {
                size_t size = is.tellg();
                _breturn_log(size == -1, _str_name_of(CompileShader) + ", file size go to -1(at std::istream::tellg)!");

                is.seekg(0, std::ios::beg);
                char* shaderCode = new char[size + 1];
                is.read(shaderCode, size);
                is.close();

                shaderCode[size] = 0; // End of char* string.

                m_pSPVData.push_back(m_pCompiler->Compile(InStageType, shaderCode));

                delete[] shaderCode;
            }
            else _return_log("Error: Could not open shader file \"" + std::string(InShaderPath) + "\"");
        }
    }

    bool HasValidSPVData()
    {
        return !m_pSPVData.empty() && m_pSPVData[_index_0] != nullptr;
    }

    SPVData* GetLastSPVData()
    {
        return m_pSPVData[m_pSPVData.size() - 1];
    }

    std::vector<SPVData*>& GetAllSPVData()
    {
        return m_pSPVData;
    }

private:

    ModuleLoader               m_module;

    PFGetGLSLCompilerInterface m_pInterface = nullptr;
    GLSLCompilerInterface*     m_pCompiler  = nullptr;

    std::vector<SPVData*>      m_pSPVData;
};
