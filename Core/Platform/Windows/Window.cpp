﻿/*********************************************************************
 *  Window.cpp
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 *********************************************************************/

#include "Window.h"
#include "Core/Engine/Engine.h"

#if PLATFORM_WINDOW

// Windows procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	if (hWnd == NULL)
	{
		return 1;
	}

	static bool s_in_sizemove = false;
	static bool s_in_suspend = false;
	static bool s_minimized = false;
	static bool s_fullscreen = false;

	switch (message)
	{
	case WM_PAINT:
		if (s_in_sizemove)
		{
			Engine::Get()->Tick();
		}
		else
		{
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
		}
		break;

	case WM_MOVE:
		Engine::Get()->OnWindowMoved();
		break;

	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
		{
			if (!s_minimized)
			{
				s_minimized = true;
				if (!s_in_suspend)
					Engine::Get()->OnSuspending();
					s_in_suspend = true;
			}
		}
		else if (s_minimized)
		{
			s_minimized = false;
			if (s_in_suspend)
				Engine::Get()->OnResuming();
				s_in_suspend = false;
		}
		else if (!s_in_sizemove)
		{
			Engine::Get()->OnWindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
		}
		break;

	case WM_ENTERSIZEMOVE:
		s_in_sizemove = true;
		break;

	case WM_EXITSIZEMOVE:
		s_in_sizemove = false;

		RECT rc;
		GetClientRect(hWnd, &rc);

		Engine::Get()->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
		break;

	case WM_GETMINMAXINFO:
	{
		auto info = reinterpret_cast<MINMAXINFO*>(lParam);
		info->ptMinTrackSize.x = 320;
		info->ptMinTrackSize.y = 200;
	}
	break;

	case WM_ACTIVATEAPP:
		if (wParam)
		{
			Engine::Get()->OnActivated();
		}
		else
		{
			Engine::Get()->OnDeactivated();
		}
		break;

	case WM_POWERBROADCAST:
		switch (wParam)
		{
		case PBT_APMQUERYSUSPEND:
			if (!s_in_suspend)
				Engine::Get()->OnSuspending();
				s_in_suspend = true;
			return TRUE;

		case PBT_APMRESUMESUSPEND:
			if (!s_minimized)
			{
				if (s_in_suspend)
					Engine::Get()->OnResuming();
					s_in_suspend = false;
			}
			return TRUE;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_SYSKEYDOWN:
		if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
		{
			// Implements the classic ALT+ENTER fullscreen toggle
			if (s_fullscreen)
			{
				SetWindowLongPtr(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
				SetWindowLongPtr(hWnd, GWL_EXSTYLE, 0);

				WindowDesc windowDesc = Engine::Get()->GetWindowDesc();

				ShowWindow(hWnd, SW_SHOWNORMAL);

				SetWindowPos(hWnd, HWND_TOP, 0, 0, windowDesc.Width, windowDesc.Height, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
			}
			else
			{
				SetWindowLongPtr(hWnd, GWL_STYLE, 0);
				SetWindowLongPtr(hWnd, GWL_EXSTYLE, WS_EX_TOPMOST);

				SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

				ShowWindow(hWnd, SW_SHOWMAXIMIZED);
			}

			s_fullscreen = !s_fullscreen;
		}
		break;

	case WM_MENUCHAR:
		// A menu is active and the user presses a key that does not correspond
		// to any mnemonic or accelerator key. Ignore so we don't produce an error beep.
		return MAKELRESULT(0, MNC_CLOSE);

	case WM_KEYDOWN:
		Engine::Get()->OnKeyDown(GET_KEYSTATE_WPARAM(wParam));
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		default:
			break;
		}
		break;
	case WM_KEYUP:
		Engine::Get()->OnKeyUp(GET_KEYSTATE_WPARAM(wParam));
		break;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		Engine::Get()->OnMouseDown(GET_KEYSTATE_WPARAM(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;

	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		Engine::Get()->OnMouseUp(GET_KEYSTATE_WPARAM(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;

	case WM_MOUSEMOVE:
		Engine::Get()->OnMouseMove(GET_KEYSTATE_WPARAM(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;

	case WM_MOUSEWHEEL:
		// x, y is not the coordinate of the cursor but pointer.
		Engine::Get()->OnMouseWheel(GET_KEYSTATE_WPARAM(wParam), GET_WHEEL_DELTA_WPARAM(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;

	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

_impl_create_interface(Window)

// Class Window.
Window::Window() : 
	bIsValid      ( false ),
	m_pHinstance  (nullptr),
	m_pHwnd       (nullptr)
{
	m_defaultDesc.Width  = 1280;
	m_defaultDesc.Height = 720;
}

Window::~Window()
{

}

void Window::Init()
{
	// Register class and create window
	{
		m_pHinstance = GetModuleHandle(NULL);

		// Register class
		WNDCLASSEXW wcex = {};
		wcex.cbSize = sizeof(WNDCLASSEXW);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.hInstance = (HINSTANCE)m_pHinstance;
		wcex.hIcon = LoadIconW((HINSTANCE)m_pHinstance, L"IDI_ICON");
		wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszClassName = L"VkJayouEngineWindowClass";
		wcex.hIconSm = LoadIconW(wcex.hInstance, L"IDI_ICON");

		if (!RegisterClassExW(&wcex))
		{
			_log_error("Register Class Failed When Create Window!", "RegisterClassExW");
			Engine::Get()->RequireExit(1);
		}

		// Create window
		uint32 w, h;
		w = m_defaultDesc.Width;
		h = m_defaultDesc.Height;

		RECT rc = { 0, 0, static_cast<LONG>(w), static_cast<LONG>(h) };

		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

		m_pHwnd = CreateWindowExW(0, L"VkJayouEngineWindowClass", L"VkJayouEngine", WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, (HINSTANCE)m_pHinstance, nullptr);

		// TODO: Change to CreateWindowExW(WS_EX_TOPMOST, L"JayouEngineWindowClass", L"JayouEngine", WS_POPUP,
		// to default to fullscreen.

		if (!m_pHwnd)
		{
			_log_error("Create Window Failed!", "CreateWindowExW");
			Engine::Get()->RequireExit(1);
		}

		// TODO: Change nCmdShow to SW_SHOWMAXIMIZED to default to fullscreen.

		//SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(m_app.get()));

		//GetClientRect(m_pHwnd, &rc);
	}

	bIsValid = true;
}

void* Window::GetHinstance() const
{
	return m_pHinstance;
}

void* Window::GetHwnd() const
{
	return m_pHwnd;
}

WindowDesc Window::GetWindowDesc() const
{
	return m_defaultDesc;
}

void Window::SetWindowDesc(const WindowDesc& InWindoWDesc)
{
	m_defaultDesc = InWindoWDesc;
}

void Window::Show()
{
	ShowWindow((HWND)m_pHwnd, SW_SHOWNORMAL);

	// Main message loop
	MSG msg = {};
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Engine::Get()->Tick();
		}
	}
}

#endif