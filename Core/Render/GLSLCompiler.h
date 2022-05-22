/*********************************************************************
 *  GLSLCompiler.h
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 * 
 *  .
 *********************************************************************/

#pragma once

#include "Core/Common.h"

class ModuleLoader;

class GLSLCompiler : public IResourceHandler
{
    _declare_create_interface(GLSLCompiler)

public:

    static constexpr uint32 NumDescriptorType  = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT + 1u + 2u; // One for push_constant, one for subpass_input.
    static constexpr uint32 ResID_PushConstant = NumDescriptorType - 2;
    static constexpr uint32 ResID_SubpassInput = NumDescriptorType - 1;

    enum class ShaderType
    {
        GLSL = 0,
        HLSL,

        MAX = 0xff
    };

    struct CompileInfo
    {
        ShaderType   shader_type = ShaderType::GLSL;
        const char*  entrypoint;
        uint32       includes_count;
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
                uint8 set;
                uint8 binding;
            };

            struct
            {
                uint8 offset;
                uint8 size;
            };

            struct
            {
                uint8 input_attachment_index;
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

    ~GLSLCompiler();

    void CompileShader(VkShaderStageFlags InStageType, const Path& InShaderPath, const CompileInfo* InCompileInfo);
    bool CheckAndParseSPVData(uint32 InMaxDescSets, std::vector<VkPushConstantRange>& OutPushConstantRanges, std::vector<std::vector<VkDescriptorSetLayoutBinding>>& OutDescSets);

    void                   FlushSPVData();
    bool                   HasValidSPVData();   
    SPVData*               GetLastSPVData();
    std::vector<SPVData*>& GetAllSPVData();

private:

    GLSLCompiler();

    ModuleLoader*                                     m_pModule;

    PFGetGLSLCompilerInterface                        m_pInterface = nullptr;
    GLSLCompilerInterface*                            m_pCompiler  = nullptr;

    std::vector<SPVData*>                             m_pSPVData;
};
