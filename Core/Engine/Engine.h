/*********************************************************************
 *  Engine.h
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 * 
 *  .
 *********************************************************************/

#pragma once

#include "Core/Base/BaseType.h"

class BaseLayer;
class Window;
class Scene;

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
	void   Run();
	void   Tick();

	void   RequireExit     (int32 InCode);
	void   CacheModuleInfo (const ModuleInfo& InModuleInfo);

	BaseLayer* GetBaseLayer();
	Window*    GetWindow();
	Scene*     GetScene();

	string GetModulePath() const;
	string GetModuleName() const;

	WindowDesc GetWindowDesc() const;

private:

	Engine() = default;
};