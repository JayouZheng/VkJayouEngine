//
// ModuleLoader.cpp
//

#include "ModuleLoader.h"

_impl_create_interface(ModuleLoader)

ModuleLoader::ModuleLoader() :
	m_pModule    (nullptr),
	m_bResult    (_false),
	m_modulePath (Path(IF_Init))
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

bool ModuleLoader::Load(const Path& InModulePath)
{
	m_modulePath = InModulePath;
#if PLATFORM_WINDOW
	m_pModule = LoadLibraryA(InModulePath.ToCString());
#endif

	if (m_pModule == nullptr)
	{
		_log_error("Fail to load module [" + InModulePath.ToString() + "]!", LogSystem::Category::ModuleLoader);
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
			_log_error("Fail to free module [" + m_modulePath.ToString() + "]!", LogSystem::Category::ModuleLoader);
			return;
		}
	}
	else
	{
		_log_error("Fail to free module [" + m_modulePath.ToString() + "], the module was not load!", LogSystem::Category::ModuleLoader);
		return;
	}
}
