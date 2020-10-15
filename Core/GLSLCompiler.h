//
// GLSLCompiler.h
//

#pragma once

#include "ModuleLoader.h"

class GLSLCompiler
{

public:

    static const uint32 NumDescriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT + 1u + 2u; // One for push_constant, one for subpass_input.
    static const uint32_t ResID_PushConstant = NumDescriptorType - 2;
    static const uint32_t ResID_SubpassInput = NumDescriptorType - 1;

    enum class ShaderType
    {
        GLSL = 0,
        HLSL,

        MAX = 0xff
    };//enum class ShaderType

    struct CompileInfo
    {
        ShaderType shader_type = ShaderType::GLSL;
        uint32_t includes_count;
        const char** includes;
    };

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

        union
        {
            struct
            {
                uint8_t set;
                uint8_t binding;
            };

            struct
            {
                uint8_t offset;
                uint8_t size;
            };

            struct
            {
                uint8_t input_attachment_index;
            };
        };

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
        uint32  shader_stage;

        ShaderStageData input, output;
        ShaderResourceData resource[NumDescriptorType];
    };

    struct GLSLCompilerInterface
    {
        bool        (*Init)    ();
        void        (*Close)   ();

        uint32      (*GetType) (const char* ext_name);
        SPVData*    (*Compile) (const uint32_t stage, const char* source, const CompileInfo* compile_info);
        SPVData*    (*CompileFromPath)(const uint32_t stage, const char* path, const CompileInfo* compile_info);

        void        (*Free)    (SPVData*);
    };

    typedef GLSLCompilerInterface* (*PFGetGLSLCompilerInterface)();


    GLSLCompiler()
    {
        m_module.Load(Global::GetModulePath() + "ThirdParty/CMBuild/GLSLCompiler/" + _platform + "/GLSLCompiler.dll");

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

    void CompileShader(VkShaderStageFlags InStageType, const std::string& InShaderPath, const CompileInfo* InCompileInfo)
    {
        if (m_pCompiler != nullptr)
        {
            m_pSPVData.push_back(m_pCompiler->CompileFromPath(InStageType, InShaderPath.data(), InCompileInfo));
        }
        else _return_log("The GLSLCompiler has not been Init!");
    }

    bool HasValidSPVData()
    {
        return !m_pSPVData.empty() && m_pSPVData[_index_0] != nullptr;
    }

    SPVData* GetLastSPVData()
    {
        return m_pSPVData.empty() ? nullptr : m_pSPVData[m_pSPVData.size() - 1];
    }

    std::vector<SPVData*>& GetAllSPVData()
    {
        return m_pSPVData;
    }

private:

    ModuleLoader                                      m_module;

    PFGetGLSLCompilerInterface                        m_pInterface = nullptr;
    GLSLCompilerInterface*                            m_pCompiler  = nullptr;

    std::vector<SPVData*>                             m_pSPVData;
};
