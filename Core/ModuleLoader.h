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

	bool Load(const std::string& InModuleName)
	{
		m_moduleName = InModuleName;
#if PLATFORM_WINDOW
		m_pModule = LoadLibraryA(InModuleName.c_str());
#endif

		if (m_pModule == nullptr)
		{
			LogSystem::LogError("Fail to load module [" + InModuleName + "]!", LogSystem::Category::ModuleLoader);
			return false;
		}
		return true;
	}

	void Free()
	{	
		if (m_pModule != nullptr)
		{
#if PLATFORM_WINDOW	
			m_bResult = FreeLibrary((HMODULE)m_pModule);
#endif
			if (m_bResult == _false)
			{
				LogSystem::LogError("Fail to free module [" + m_moduleName + "]!", LogSystem::Category::ModuleLoader);
				return;
			}
		}
		else
		{
			LogSystem::LogError("Fail to free module [" + m_moduleName + "], the module was not load!", LogSystem::Category::ModuleLoader);
			return;
		}
	}

	template<typename T>
	T GetInterface(const std::string& InInterfaceName)
	{
		if (m_pModule != nullptr)
		{
#if PLATFORM_WINDOW	
			T api = nullptr;
			api = (T)GetProcAddress((HMODULE)m_pModule, InInterfaceName.c_str());

			if (api == nullptr)
			{
				LogSystem::LogError("Fail to find API [" + InInterfaceName + "]!", LogSystem::Category::ModuleLoader);
				return nullptr;
			}
			return api;
#endif
		}
		else
		{
			LogSystem::LogError("Fail to find API [" + InInterfaceName + "], the module [" + m_moduleName + "] was not load!", LogSystem::Category::ModuleLoader);
			return nullptr;
		}
	}

private:

	std::string m_moduleName;

	void*       m_pModule = nullptr;
	iBool       m_bResult = _false;
};