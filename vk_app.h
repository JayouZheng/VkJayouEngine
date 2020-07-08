//
// vk_app.h
//

#pragma once

#include "app_allocator.h"

class vk_app
{

public:

	vk_app();
	~vk_app();

public:

	void Begin();
	void End();

public:

	app_allocator m_allocator;
};
