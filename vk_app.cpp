//
// vk_app.cpp
//

#include "Core/BaseLayer.h"
#include "Core/Global.h"

#include "vk_app.h"
#include "app_allocator.h"

vk_app::vk_app()
{

}

vk_app::~vk_app()
{
	End();
}

void vk_app::Begin()
{
	Global::SetAllocator(new app_allocator);
	BaseLayer base(Global::GetAllocator());
	base.Init();
}

void vk_app::End()
{
	
}
