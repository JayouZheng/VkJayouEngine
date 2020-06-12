//
// vk_app.h
//

#pragma once

#include "vk_ptr.h"

class vk_app
{

public:

	vk_app() {}
	~vk_app() {}

	void Init();

protected:

	vk_ptr<VkInstance> m_instance;
};