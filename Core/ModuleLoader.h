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
#if PLATFORM_WINDOW
		m_pModule = LoadLibraryA(InModuleName.c_str());
#endif
		_breturn_log(m_pModule == nullptr, "Fail to load module [" + InModuleName + "]!");
	}

	void Free()
	{	
		if (m_pModule != nullptr)
		{
#if PLATFORM_WINDOW	
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
#if PLATFORM_WINDOW	
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

	std::string m_moduleName;

	void*       m_pModule = nullptr;
	iBool       m_bResult = _false;
};