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
		if (m_bResult == _false)
		{
			Free();
		}
	}

	void Load(const std::string& InModuleName)
	{
		m_moduleName = InModuleName;
#if VK_USE_PLATFORM_WIN32_KHR
		m_pModule = LoadLibraryA(InModuleName.c_str());
#endif
		_breturn_log(m_pModule == nullptr, "Fail to load module [" + InModuleName + "]!");
	}

	void Free()
	{	
		if (m_pModule != nullptr)
		{
#if VK_USE_PLATFORM_WIN32_KHR	
			m_bResult = FreeLibrary((HMODULE)m_pModule);
#endif
			_breturn_log(m_bResult == _false, "Fail to free module [" + m_moduleName + "]!");
		}
		else _return_log("Fail to free module [" + m_moduleName + "], the module was not load!");
	}

	template<typename T>
	T GetInterface(const std::string& InInterfaceName)
	{
		if (m_pModule != nullptr)
		{
#if VK_USE_PLATFORM_WIN32_KHR	
			T api = nullptr;
			api = (T)GetProcAddress((HMODULE)m_pModule, InInterfaceName.c_str());
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

	void* m_pModule = nullptr;
	std::string m_moduleName;

	iBool m_bResult = _false;
};