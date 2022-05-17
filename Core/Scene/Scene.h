/*********************************************************************
 *  Scene.h
 *  Copyright (C) 2022 Jayou. All Rights Reserved.
 * 
 *  Scene to render.
 *********************************************************************/

#pragma once

#include "Core/Common.h"

class Scene : public IResourceHandler
{
	_declare_create_interface(Scene)

public:

	Scene();
	~Scene();

	void Init();
	void Update(const TimerUtil::GameTimer& InGameTimer);
	void Render(const TimerUtil::GameTimer& InGameTimer);
};
