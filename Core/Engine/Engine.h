/*********************************************************************
 *  Engine.h
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 * 
 *  .
 *********************************************************************/

#pragma once

#include "Core/Base/BaseType.h"

class BaseLayer;

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

	void   RequireExit     (int32 InCode);
	void   CacheModuleInfo (const ModuleInfo& InModuleInfo);

	BaseLayer* GetBaseLayer();

	string GetModulePath() const;
	string GetModuleName() const;

private:

	Engine() = default;
};