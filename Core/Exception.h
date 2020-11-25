//
// Exception.h
//

#pragma once

#include "Core/TypeDef.h"
#include "Core/Utility/LogSystem.h"

class vkException
{
public:
	vkException() = default;
	vkException(
		VkResult     InResult,
		const char*  InFuncName,
		const char*  InFilename,
		int          InLineNumber) :
		m_result    (InResult),
		m_funcName  (InFuncName),
		m_fileName  (InFilename),
		m_lineNumber(InLineNumber)
	{}

    static string VkResultToString(VkResult InResult)
    {
#define VK_RESULT_TO_STRING(name)   if (InResult==name) return _name_of(name);

		VK_RESULT_TO_STRING(VK_SUCCESS);
		VK_RESULT_TO_STRING(VK_NOT_READY);
        VK_RESULT_TO_STRING(VK_TIMEOUT);
        VK_RESULT_TO_STRING(VK_EVENT_SET);
        VK_RESULT_TO_STRING(VK_EVENT_RESET);
        VK_RESULT_TO_STRING(VK_INCOMPLETE);
        VK_RESULT_TO_STRING(VK_ERROR_OUT_OF_HOST_MEMORY);
        VK_RESULT_TO_STRING(VK_ERROR_OUT_OF_DEVICE_MEMORY);
        VK_RESULT_TO_STRING(VK_ERROR_INITIALIZATION_FAILED);
        VK_RESULT_TO_STRING(VK_ERROR_DEVICE_LOST);
        VK_RESULT_TO_STRING(VK_ERROR_MEMORY_MAP_FAILED);
        VK_RESULT_TO_STRING(VK_ERROR_LAYER_NOT_PRESENT);
        VK_RESULT_TO_STRING(VK_ERROR_EXTENSION_NOT_PRESENT);
        VK_RESULT_TO_STRING(VK_ERROR_FEATURE_NOT_PRESENT);
        VK_RESULT_TO_STRING(VK_ERROR_INCOMPATIBLE_DRIVER);
        VK_RESULT_TO_STRING(VK_ERROR_TOO_MANY_OBJECTS);
        VK_RESULT_TO_STRING(VK_ERROR_FORMAT_NOT_SUPPORTED);
        VK_RESULT_TO_STRING(VK_ERROR_FRAGMENTED_POOL);
        VK_RESULT_TO_STRING(VK_ERROR_UNKNOWN);
        VK_RESULT_TO_STRING(VK_ERROR_OUT_OF_POOL_MEMORY);
        VK_RESULT_TO_STRING(VK_ERROR_INVALID_EXTERNAL_HANDLE);
        VK_RESULT_TO_STRING(VK_ERROR_FRAGMENTATION);
        VK_RESULT_TO_STRING(VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS);
        VK_RESULT_TO_STRING(VK_ERROR_SURFACE_LOST_KHR);
        VK_RESULT_TO_STRING(VK_ERROR_NATIVE_WINDOW_IN_USE_KHR);
        VK_RESULT_TO_STRING(VK_SUBOPTIMAL_KHR);
        VK_RESULT_TO_STRING(VK_ERROR_OUT_OF_DATE_KHR);
        VK_RESULT_TO_STRING(VK_ERROR_INCOMPATIBLE_DISPLAY_KHR);
        VK_RESULT_TO_STRING(VK_ERROR_VALIDATION_FAILED_EXT);
        VK_RESULT_TO_STRING(VK_ERROR_INVALID_SHADER_NV);
        VK_RESULT_TO_STRING(VK_ERROR_INCOMPATIBLE_VERSION_KHR);
        VK_RESULT_TO_STRING(VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT);
        VK_RESULT_TO_STRING(VK_ERROR_NOT_PERMITTED_EXT);
        VK_RESULT_TO_STRING(VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT);
        VK_RESULT_TO_STRING(VK_THREAD_IDLE_KHR);
        VK_RESULT_TO_STRING(VK_THREAD_DONE_KHR);
        VK_RESULT_TO_STRING(VK_OPERATION_DEFERRED_KHR);
        VK_RESULT_TO_STRING(VK_OPERATION_NOT_DEFERRED_KHR);
        VK_RESULT_TO_STRING(VK_PIPELINE_COMPILE_REQUIRED_EXT);

        return _str_unknown;
    }

	string ToString() const
	{
		return m_funcName + " failed in " + m_fileName + "; line " + std::to_string(m_lineNumber) + "; result: " + vkException::VkResultToString(m_result);
	}

	VkResult    m_result;
	string m_funcName;
	string m_fileName;
	uint32      m_lineNumber;
};

#define _vk_throw(func, func_name, file_name, file_line)             \
{                                                                    \
	VkResult result = (func);                                        \
	if (result != VK_SUCCESS)                                        \
	{                                                                \
		throw vkException(result, func_name, file_name, file_line);  \
	}                                                                \
}

template<typename TLambda>
void vk_try(const TLambda& InLambda, const char* InFuncName, const char* InFileName, uint32 InFileLine)
{
    VkResult result = (InLambda);
    if (result != VK_SUCCESS)
    {
        _log_error(vkException(result, InFuncName, InFileName, InFileLine).ToString(), LogSystem::Category::VulkanAPI);
    }
}

#define _vk_try(x) vk_try(x, _name_of(x), __FILE__, __LINE__);