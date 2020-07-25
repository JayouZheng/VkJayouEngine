//
// vk_util.h
//

#pragma once

#include "SmartPtr.h"

#include "Platform.h"
#include "TypeDef.h"
#include "vulkan/vulkan.hpp"

#pragma comment(lib, "vulkan-1.lib")

class vk_exception
{
public:
	vk_exception() = default;
	vk_exception(VkResult hr,
		const std::wstring& functionName,
		const std::wstring& filename,
		int lineNumber) :
		m_result(hr),
		m_funcName(functionName),
		m_fileName(filename),
		m_lineNumber(lineNumber)
	{}

	std::wstring ToString() const
	{
		return m_funcName + L" failed in " + m_fileName + L"; line " + std::to_wstring(m_lineNumber) + L"; error: " + std::to_wstring((int)m_result);
	}

	VkResult m_result = VK_SUCCESS;
	std::wstring m_funcName;
	std::wstring m_fileName;
	int m_lineNumber = -1;
};

#if VK_USE_PLATFORM_WIN32_KHR

#include <windows.h>

inline std::wstring to_wstring(const std::string& str)
{
	int bufferlen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	wchar_t* buffer = new wchar_t[bufferlen];
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buffer, bufferlen);
	std::wstring wstr(buffer);
	delete[] buffer;
	return wstr;
}

#endif

#define _vk_throw(_func, _func_name, _file_name, _file_line)            \
{                                                                       \
	VkResult result = (_func);                                          \
	if (result != VK_SUCCESS)                                           \
	{                                                                   \
		throw vk_exception(result, _func_name, _file_name, _file_line); \
	}                                                                   \
}

template<typename TLambda>
void vk_try(const TLambda& lambda, const wchar_t* func_name, const std::wstring& file_name, int file_line)
{
	try
	{
		_vk_throw(lambda, func_name, file_name, file_line);
	}
	catch (const vk_exception& e)
	{
		_cmd_print_line_ws(e.ToString().data());
	}
}

#define _vk_try(x) vk_try(x, _name_of(x), to_wstring(__FILE__), __LINE__);