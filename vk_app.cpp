//
// vk_app.cpp
//

#include "vk_app.h"
#include "Core/Base/BaseLayer.h"
#include "Core/Engine/Engine.h"

vk_app::vk_app()
{

}

vk_app::~vk_app()
{
	End();
}

void vk_app::Begin()
{
	Engine::Get()->Init();
	Engine::Get()->Run();
}

void vk_app::End()
{
	Engine::Get()->RequireExit(0);
}
