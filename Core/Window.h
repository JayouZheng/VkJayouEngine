//
// Window.h
//

#pragma once

#include "Platform.h"
#include "TypeDef.h"

#if PLATFORM_WINDOW

class Window
{

public:

	struct WindowDesc
	{		
		uint32 Width;
		uint32 Height;
	};

public:

	Window();
	Window(const WindowDesc& InWindoWDesc);
	~Window();

public:

	bool Init();

public:

	void* GetHinstance() const;
	void* GetHwnd() const;

	WindowDesc GetWindowDesc() const;
	void SetWindowDesc(const WindowDesc& InWindoWDesc);

	void Show();

protected:

	void* m_pHinstance = nullptr;
	void* m_pHwnd = nullptr;

	WindowDesc m_defaultDesc = 
	{
		1280,
		720
	};
};

#endif