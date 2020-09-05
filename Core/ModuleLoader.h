//
// ModuleLoader.h
//

#pragma once

#include "Global.h"

class ModuleLoader
{

public:

	ModuleLoader() {}

	~ModuleLoader()
	{
		if (m_result == _false)
		{
			Free();
		}
	}

	void Load(const std::string& InModuleName)
	{
		m_moduleName = InModuleName;
#if VK_USE_PLATFORM_WIN32_KHR
		m_module = LoadLibraryA(InModuleName.c_str());
#endif
		_breturn_log(m_module == nullptr, "Fail to load module [" + InModuleName + "]!");
	}

	void Free()
	{	
		if (m_module != nullptr)
		{
#if VK_USE_PLATFORM_WIN32_KHR	
			m_result = FreeLibrary((HMODULE)m_module);
#endif
			_breturn_log(m_result == _false, "Fail to free module [" + m_moduleName + "]!");
		}
		else _return_log("Fail to free module [" + m_moduleName + "], the module was not load!");
	}

	template<typename T>
	T GetInterface(const std::string& InInterfaceName)
	{
		if (m_module != nullptr)
		{
#if VK_USE_PLATFORM_WIN32_KHR	
			T api = nullptr;
			api = (T)GetProcAddress((HMODULE)m_module, InInterfaceName.c_str());
			_breturnx_log(api == nullptr, nullptr, "Fail to find API [" + InInterfaceName + "]!");
			return api;
#endif
		}
		else
		{
			_returnx_log(nullptr, "Fail to find API [" + InInterfaceName + "], the module [" + m_moduleName + "] was not load!");
		}
	}

private:

	void* m_module = nullptr;
	std::string m_moduleName;

	iBool m_result = _false;
};