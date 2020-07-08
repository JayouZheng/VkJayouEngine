//
// vk_app.cpp
//

#include "vk_app.h"
#include "BaseLayer.h"

vk_app::vk_app()
{

}

vk_app::~vk_app()
{
	End();
}

void vk_app::Begin()
{
	BaseLayer base(&m_allocator);
	base.Init();
}

void vk_app::End()
{
	
}
