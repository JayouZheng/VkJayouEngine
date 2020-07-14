//
// main.cpp
//

#include "vk_app.h"
#include "vk_util.h"

int main()
{
	_cmd_print_line("Hello Vulkan!\n");

	{
		vk_app app;
		app.Begin();
	}

	system("pause");
}
