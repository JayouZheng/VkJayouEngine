//
// Engine.h
//

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