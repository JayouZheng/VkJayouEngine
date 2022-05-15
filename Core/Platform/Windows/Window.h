/*********************************************************************
 *  Window.h
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 * 
 *  .
 *********************************************************************/

#pragma once

#include "Core/TypeDef.h"
#include "Core/Platform/Platform.h"
#include "Core/Base/Interface/IResourceHandler.h"

#if PLATFORM_WINDOW

class Window : public IResourceHandler
{
	_declare_create_interface(Window)

protected:

	bool bIsValid;

	void* m_pHinstance;
	void* m_pHwnd;

	WindowDesc m_defaultDesc;

	Window();

public:

	virtual ~Window();

	void Init();

	bool IsValid() const { return bIsValid; }

	void* GetHinstance() const;
	void* GetHwnd() const;

	WindowDesc GetWindowDesc() const;
	void SetWindowDesc(const WindowDesc& InWindoWDesc);

	void Show();

};

#endif