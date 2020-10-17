//
// main.cpp
//

#include "Core/TypeDef.h"
#include "vk_app.h"

 int main()
 {
 	_cmd_print_line("Hello Vulkan!\n");

#ifdef RunApp
 
 	{
 		vk_app app;
 		app.Begin();
 	}

#endif
 
 	system("pause");
 }
