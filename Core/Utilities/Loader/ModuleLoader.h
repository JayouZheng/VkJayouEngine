/*********************************************************************
 *  ModuleLoader.h
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 * 
 *  This library provide some functions to load DLL dynamically.
 *********************************************************************/

#pragma once

#include "Core/TypeDef.h"
#include "../Parser/PathParser.h"
#include "Core/Base/Interface/IResourceHandler.h"

class ModuleLoader : public IResourceHandler
{
	_declare_create_interface(ModuleLoader)

public:
	
	/**
	 *  Dynamic load DLL directlty.
	 * 
	 *  @param  InPath  the path of DLL
	 * 
	 *  @return true if success, otherwise false.
	 */
	bool Load(const Path& InPath);
	
	/**
	 *  Free/Unload DLL directly.
	 */
	void Free();

	/**
	 *  Resolve a interface through a specific name.
	 *
	 *  @param  InName  the interface name.
	 *
	 *  @return specific type of interface.
	 */
	template<typename T>
	T GetInterface(const string& InName);

	~ModuleLoader();

private:

	ModuleLoader();

	void*       m_pModule;         ///< Cache the instance of DLL.
	iBool       m_bResult;         ///< Cache the result of Free().
	Path        m_modulePath;	   ///< Cache the path of loaded DLL.
};

#include "ModuleLoader.inl"
