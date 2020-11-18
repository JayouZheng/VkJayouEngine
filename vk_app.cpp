//
// vk_app.cpp
//

#include "vk_app.h"
#include "Core/Base/BaseLayer.h"
#include "Core/Base/ResourcePool.h"

vk_app::vk_app()
{

}

vk_app::~vk_app()
{
	End();
}

void vk_app::Begin()
{
	BaseLayer* base = BaseLayer::Create(nullptr);
	ResourcePool::Get()->Push(base);
	base->Init();
}

void vk_app::End()
{
	ResourcePool::Get()->Free();
}
