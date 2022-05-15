/*********************************************************************
 *  Scene.cpp
 *  Copyright (C) 2022 Jayou. All Rights Reserved.
 * 
 *  Scene to render.
 *********************************************************************/

#include "Scene.h"

_impl_create_interface(Scene)

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::Init()
{
}

void Scene::Update(const TimerUtil::GameTimer* InGameTimer)
{
	static int32 count = 0;
	count++;
}

void Scene::Render(const TimerUtil::GameTimer* InGameTimer)
{
}
