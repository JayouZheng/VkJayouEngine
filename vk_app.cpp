//
// vk_app.cpp
//

#include "vk_app.h"
#include "BaseLayer.h"
#include "Global.h"
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
	Global::SetGlobalAllocator(new app_allocator);
	BaseLayer base(Global::GetGlobalAllocator());
	base.Init();
}

void vk_app::End()
{
	
}
