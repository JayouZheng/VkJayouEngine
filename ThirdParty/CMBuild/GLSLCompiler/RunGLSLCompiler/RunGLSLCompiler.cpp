//
// RunGLSLCompiler.cpp
//

#pragma region Prerequest

#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#ifdef _DEBUG
#define SlnConfig "Debug"
#else
#define SlnConfig "Release"
#endif

using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;

using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

using byte = uint8;

using iBool = int32;
using uBool = uint32;

#define _index_0      0

#define _false        0
#define _true         1

#define _name_of(x)   #x
#define _str_name_of(x) std::string(_name_of(x))

#define _cmd_print_line(str) std::cout << str << std::endl

namespace Global
{
	void CacheLog(const std::string& InLog)
	{
		_cmd_print_line(InLog);
	}
}

#define _return_log(log) { Global::CacheLog(log); return; }
#define _breturn_log(b, log) if (b) { Global::CacheLog(log); return; }
#define _returnx_log(ret, log) { Global::CacheLog(log); return ret; }
#define _breturnx_log(b, ret, log) if(b) { Global::CacheLog(log); return ret; }

class ModuleLoader
{

public:

	ModuleLoader() {}

	~ModuleLoader()
	{
		if (m_bResult == _false)
		{
			Free();
		}
	}

	void Load(const std::string& InModuleName)
	{
		m_moduleName = InModuleName;

		m_pModule = LoadLibraryA(InModuleName.c_str());

		_breturn_log(m_pModule == nullptr, "Fail to load module [" + InModuleName + "]!");
	}

	void Free()
	{
		if (m_pModule != nullptr)
		{

			m_bResult = FreeLibrary((HMODULE)m_pModule);

			_breturn_log(m_bResult == _false, "Fail to free module [" + m_moduleName + "]!");
		}
		else _return_log("Fail to free module [" + m_moduleName + "], the module was not load!");
	}

	template<typename T>
	T GetInterface(const std::string& InInterfaceName)
	{
		if (m_pModule != nullptr)
		{
			T api = nullptr;
			api = (T)GetProcAddress((HMODULE)m_pModule, InInterfaceName.c_str());
			_breturnx_log(api == nullptr, nullptr, "Fail to find API [" + InInterfaceName + "]!");
			return api;
		}
		else
		{
			_returnx_log(nullptr, "Fail to find API [" + InInterfaceName + "], the module [" + m_moduleName + "] was not load!");
		}
	}

private:

	void* m_pModule = nullptr;
	std::string m_moduleName;

	iBool m_bResult = _false;
};

typedef enum VkShaderStageFlagBits {
    VK_SHADER_STAGE_VERTEX_BIT = 0x00000001,
    VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT = 0x00000002,
    VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT = 0x00000004,
    VK_SHADER_STAGE_GEOMETRY_BIT = 0x00000008,
    VK_SHADER_STAGE_FRAGMENT_BIT = 0x00000010,
    VK_SHADER_STAGE_COMPUTE_BIT = 0x00000020,
    VK_SHADER_STAGE_ALL_GRAPHICS = 0x0000001F,
    VK_SHADER_STAGE_ALL = 0x7FFFFFFF,
    VK_SHADER_STAGE_RAYGEN_BIT_KHR = 0x00000100,
    VK_SHADER_STAGE_ANY_HIT_BIT_KHR = 0x00000200,
    VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR = 0x00000400,
    VK_SHADER_STAGE_MISS_BIT_KHR = 0x00000800,
    VK_SHADER_STAGE_INTERSECTION_BIT_KHR = 0x00001000,
    VK_SHADER_STAGE_CALLABLE_BIT_KHR = 0x00002000,
    VK_SHADER_STAGE_TASK_BIT_NV = 0x00000040,
    VK_SHADER_STAGE_MESH_BIT_NV = 0x00000080,
    VK_SHADER_STAGE_RAYGEN_BIT_NV = VK_SHADER_STAGE_RAYGEN_BIT_KHR,
    VK_SHADER_STAGE_ANY_HIT_BIT_NV = VK_SHADER_STAGE_ANY_HIT_BIT_KHR,
    VK_SHADER_STAGE_CLOSEST_HIT_BIT_NV = VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR,
    VK_SHADER_STAGE_MISS_BIT_NV = VK_SHADER_STAGE_MISS_BIT_KHR,
    VK_SHADER_STAGE_INTERSECTION_BIT_NV = VK_SHADER_STAGE_INTERSECTION_BIT_KHR,
    VK_SHADER_STAGE_CALLABLE_BIT_NV = VK_SHADER_STAGE_CALLABLE_BIT_KHR,
    VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM = 0x7FFFFFFF
} VkShaderStageFlagBits;

using VkShaderStageFlags = uint32;

typedef enum VkDescriptorType {
    VK_DESCRIPTOR_TYPE_SAMPLER = 0,
    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER = 1,
    VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE = 2,
    VK_DESCRIPTOR_TYPE_STORAGE_IMAGE = 3,
    VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER = 4,
    VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER = 5,
    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER = 6,
    VK_DESCRIPTOR_TYPE_STORAGE_BUFFER = 7,
    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC = 8,
    VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC = 9,
    VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT = 10,
    VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT = 1000138000,
    VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR = 1000165000,
    VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR,
    VK_DESCRIPTOR_TYPE_MAX_ENUM = 0x7FFFFFFF
} VkDescriptorType;

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
        m_module.Load("../../.GLSLCompiler/out/Windows_64_" + std::string(SlnConfig) + "/GLSLCompiler.dll");

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
        return m_pSPVData.empty() ? nullptr : m_pSPVData[m_pSPVData.size() - 1];
    }

    std::vector<SPVData*>& GetAllSPVData()
    {
        return m_pSPVData;
    }

private:

    ModuleLoader               m_module;

    PFGetGLSLCompilerInterface m_pInterface = nullptr;
    GLSLCompilerInterface*     m_pCompiler = nullptr;

    std::vector<SPVData*>      m_pSPVData;
};

#pragma endregion

int main()
{
    _cmd_print_line("Hello GLSLCompiler!\n");

    GLSLCompiler compiler;
    compiler.CompileShader(VK_SHADER_STAGE_FRAGMENT_BIT, R"(C:\Users\zhengjianyao\Desktop\pbrtexture.hlsl)");
    GLSLCompiler::SPVData* spvData = compiler.GetLastSPVData();

    system("pause");
}
