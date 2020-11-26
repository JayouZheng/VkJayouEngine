/*********************************************************************
 *  ModuleLoader.h
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 * 
 *  This library provide some functions to load .dll dynamically.
 *********************************************************************/

#pragma once

#include "Core/Common.h"

class ModuleLoader : public IResourceHandler
{
	_declare_create_interface(ModuleLoader)

public:

	~ModuleLoader();
	bool Load(const Path& InPath);
	void Free();

	template<typename T>
	T GetInterface(const string& InInterfaceName);

private:

	ModuleLoader();

	void*       m_pModule;
	iBool       m_bResult;
	Path        m_modulePath;	
};

#include "ModuleLoader.inl"
