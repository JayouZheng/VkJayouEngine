//
// ModuleLoader.h
//

#pragma once

#include "Core/Common.h"

class ModuleLoader : public IResourceHandler
{
	_declare_create_interface(ModuleLoader)

public:

	~ModuleLoader();
	bool Load(const std::string& InModuleName);
	void Free();

	template<typename T>
	T GetInterface(const std::string& InInterfaceName);

private:

	ModuleLoader();

	void*       m_pModule;
	iBool       m_bResult;
	std::string m_moduleName;	
};

// #include "ModuleLoader.inl"

template<typename T>
T ModuleLoader::GetInterface(const std::string& InInterfaceName)
{
	if (m_pModule != nullptr)
	{
#if PLATFORM_WINDOW	
		T api = nullptr;
		api = (T)GetProcAddress((HMODULE)m_pModule, InInterfaceName.c_str());

		if (api == nullptr)
		{
			_log_error("Fail to find API [" + InInterfaceName + "]!", LogSystem::Category::ModuleLoader);
			return nullptr;
		}
		return api;
#endif
	}
	else
	{
		_log_error("Fail to find API [" + InInterfaceName + "], the module [" + m_moduleName + "] was not load!", LogSystem::Category::ModuleLoader);
		return nullptr;
	}
}