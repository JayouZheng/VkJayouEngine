/*********************************************************************
 *  Engine.h
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 * 
 *  .
 *********************************************************************/

#pragma once

#include "Core/Base/BaseType.h"

class Engine
{

public:

	struct ModuleInfo
	{
		string Path;
		string Name;
	};

	~Engine();

	static Engine*& Get();

	void   Init();
	void   Exit();

	void   CacheModuleInfo(const ModuleInfo& InModuleInfo);
	string GetModulePath() const;
	string GetModuleName() const;

private:

	Engine() = default;
	
	ModuleInfo m_moduleInfo;
};