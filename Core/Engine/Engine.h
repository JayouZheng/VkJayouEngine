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

	void Init();
	void Run();
	void Tick();

	void RequireExit(int32 InCode);
	void CacheModuleInfo(const ModuleInfo& InModuleInfo);

	BaseLayer* GetBaseLayer();
	Window* GetWindow();
	Scene* GetScene();

	string GetModulePath() const;
	string GetModuleName() const;

	WindowDesc GetWindowDesc() const;

	//---------------------------------------------------------------------------
	// Viewport intersection event handle.
	//---------------------------------------------------------------------------

	void OnWindowMoved();
	void OnWindowSizeChanged(int32 InWidth, int32 InHeight);

	void OnActivated();
	void OnDeactivated();
	void OnSuspending();
	void OnResuming();

	void OnKeyDown(int32 InKeyState);
	void OnKeyUp(int32 InKeyState);
	void OnMouseDown(int32 InMouseState, int32 InXAxis, int32 InYAxis);
	void OnMouseUp(int32 InMouseState, int32 InXAxis, int32 InYAxis);
	void OnMouseMove(int32 InMouseState, int32 InXAxis, int32 InYAxis);
	void OnMouseWheel(int32 InMouseState, int32 InWheelDelta, int32 InXAxis, int32 InYAxis);

private:

	Engine() = default;
};