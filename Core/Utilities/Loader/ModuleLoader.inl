/*********************************************************************
 *  ModuleLoader.inl
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 *********************************************************************/

#pragma once

#include "../Log/LogSystem.h"

template<typename T>
T ModuleLoader::GetInterface(const string& InName)
{
	if (m_pModule != nullptr)
	{
#if PLATFORM_WINDOW	
		T api = nullptr;
		api = (T)GetProcAddress((HMODULE)m_pModule, InName.c_str());

		if (api == nullptr)
		{
			_log_error("Fail to find API [" + InName + "]!", LogSystem::Category::ModuleLoader);
			return nullptr;
		}
		return api;
#endif
	}
	else
	{
		_log_error("Fail to find API [" + InName + "], the module [" + m_modulePath.ToString() + "] was not load!", LogSystem::Category::ModuleLoader);
		return nullptr;
	}
}
