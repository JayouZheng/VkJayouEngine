//
// vk_app.cpp
//

#include "vk_app.h"
#include "vk_util.h"

#pragma comment(lib, "vulkan-1.lib")

vk_app app;

int main()
{
    _cmd_print_line("Hello Vulkan!\n");

	app.Init();

	system("pause");
}

void vk_app::Init()
{
	VkApplicationInfo appInfo = {};
	VkInstanceCreateInfo instanceCreateInfo = {};

	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Application";
	appInfo.applicationVersion = 1;
	appInfo.apiVersion = VK_MAKE_VERSION(1, 0, 0);

	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo = &appInfo;

	_vk_try(vkCreateInstance(&instanceCreateInfo, nullptr, m_instance));
}
