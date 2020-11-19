//
// ModuleLoader.cpp
//

#include "ModuleLoader.h"

_impl_create_interface(ModuleLoader)

ModuleLoader::ModuleLoader() :
	m_pModule    (nullptr),
	m_bResult    (_false),
	m_moduleName (_str_null)
{
	_internal_init(ModuleLoader);
}

ModuleLoader::~ModuleLoader()
{
	if (m_bResult == _false)
	{
		Free();
	}
}

bool ModuleLoader::Load(const std::string& InModuleName)
{
	m_moduleName = InModuleName;
#if PLATFORM_WINDOW
	m_pModule = LoadLibraryA(InModuleName.c_str());
#endif

	if (m_pModule == nullptr)
	{
		_log_error("Fail to load module [" + InModuleName + "]!", LogSystem::Category::ModuleLoader);
		return false;
	}
	return true;
}

void ModuleLoader::Free()
{
	if (m_pModule != nullptr)
	{
#if PLATFORM_WINDOW	
		m_bResult = FreeLibrary((HMODULE)m_pModule);
#endif
		if (m_bResult == _false)
		{
			_log_error("Fail to free module [" + m_moduleName + "]!", LogSystem::Category::ModuleLoader);
			return;
		}
	}
	else
	{
		_log_error("Fail to free module [" + m_moduleName + "], the module was not load!", LogSystem::Category::ModuleLoader);
		return;
	}
}
