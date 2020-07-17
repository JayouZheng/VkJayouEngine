//
// Window.h
//

#pragma once

#include "Platform.h"

#if VK_USE_PLATFORM_WIN32_KHR

class Window
{

public:

	struct WindowDesc
	{		
		int Width;
		int Height;
	};

public:

	Window();
	~Window();

public:

	void* GetHinstance() const;
	void* GetHwnd() const;

	void Show();

protected:

	void* m_hinstance = nullptr;
	void* m_hwnd = nullptr;

	const WindowDesc m_defaultDesc = 
	{
		1280,
		720
	};
};

#endif