/*********************************************************************
 *  Window.h
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 * 
 *  .
 *********************************************************************/

#pragma once

#include "Core/Common.h"

#if PLATFORM_WINDOW

class Window : public IResourceHandler
{
	_declare_create_interface(Window)

protected:

	void* m_pHinstance;
	void* m_pHwnd;

	struct WindowDesc
	{
		uint32 Width;
		uint32 Height;

	}m_defaultDesc;

	Window();

public:

	virtual ~Window();

	void Init();

	void* GetHinstance() const;
	void* GetHwnd() const;

	WindowDesc GetWindowDesc() const;
	void SetWindowDesc(const WindowDesc& InWindoWDesc);

	void Show();

};

#endif