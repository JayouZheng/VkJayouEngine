//
// main.cpp
//

#include "Core/TypeDef.h"
#include "vk_app.h"

int main()
{
	_cmd_print_line("Hello Vulkan!\n");

	{
		vk_app app;
		app.Begin();
	}

	system("pause");
}
