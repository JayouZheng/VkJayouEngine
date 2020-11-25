/*********************************************************************
 *  ModuleLoader.inl
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 *********************************************************************/

#pragma once

template<typename T>
T ModuleLoader::GetInterface(const string& InInterfaceName)
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
		_log_error("Fail to find API [" + InInterfaceName + "], the module [" + m_modulePath.ToString() + "] was not load!", LogSystem::Category::ModuleLoader);
		return nullptr;
	}
}
