//
// vk_app.cpp
//

#include "Core/Base/BaseLayer.h"
#include "Core/Global.h"

#include "vk_app.h"

vk_app::vk_app()
{

}

vk_app::~vk_app()
{
	End();
}

void vk_app::Begin()
{
	BaseLayer base;
	base.Init();
}

void vk_app::End()
{
	
}
