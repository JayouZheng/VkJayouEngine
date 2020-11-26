/*********************************************************************
 *  Engine.h
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 * 
 *  .
 *********************************************************************/

#pragma once

class Engine
{

private:

	Engine() = default;

public:

	~Engine();

	static Engine*& Get();

	void Init();
	void Exit();
};