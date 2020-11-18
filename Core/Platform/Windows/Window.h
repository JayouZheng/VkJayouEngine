//
// Window.h
//

#pragma once

#include "Core/TypeDef.h"
#include "Core/Platform/Platform.h"
#include "Core/Base/Interface/IResourceHandler.h"

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

	bool Init();

	void* GetHinstance() const;
	void* GetHwnd() const;

	WindowDesc GetWindowDesc() const;
	void SetWindowDesc(const WindowDesc& InWindoWDesc);

	void Show();

};

#endif