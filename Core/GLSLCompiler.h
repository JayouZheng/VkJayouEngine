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

        m_interface = m_module.GetInterface<PFGetGLSLCompilerInterface>("GetInterface");

        if (m_interface != nullptr)
        {
            m_compiler = m_interface();
            if (m_compiler != nullptr) m_compiler->Init();
        }
    }

    ~GLSLCompiler()
    {
        if (m_compiler != nullptr)
        {
            if (m_spvData != nullptr) m_compiler->Free(m_spvData);
            m_compiler->Close();
        }

        m_module.Free();
    }

    void CompileShader(VkShaderStageFlags InStageType, const std::string& InShaderPath)
    {
        if (m_compiler != nullptr)
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

                m_spvData = m_compiler->Compile(InStageType, shaderCode);

                delete[] shaderCode;
            }
            else _return_log("Error: Could not open shader file \"" + std::string(InShaderPath) + "\"");
        }
    }

    SPVData GetDuplicatedSPVData()
    {
        return m_spvData != nullptr ? *m_spvData : SPVData();
    }

    SPVData* GetSPVData()
    {
        return m_spvData;
    }

private:

    ModuleLoader               m_module;
    PFGetGLSLCompilerInterface m_interface = nullptr;

    GLSLCompilerInterface*     m_compiler  = nullptr;
    SPVData*                   m_spvData   = nullptr;
};
