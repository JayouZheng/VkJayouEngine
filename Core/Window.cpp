//
// Window.cpp
//

#include "Window.h"
#include "Global.h"

#if VK_USE_PLATFORM_WIN32_KHR

#include <WinSDKVer.h>
#define _WIN32_WINNT 0x0A00
#include <SDKDDKVer.h>

// Use the C++ standard templated min/max
#define NOMINMAX

// Include <mcx.h> if you need this
#define NOMCX

// Include <winsvc.h> if you need this
#define NOSERVICE

// WinHelp is deprecated
#define NOHELP

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <windowsx.h> // GET_X_LPARAM...

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

	void* app = nullptr;

	switch (message)
	{
	case WM_PAINT:
		if (s_in_sizemove && app)
		{
			//app->Tick();
		}
		else
		{
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
		}
		break;

	case WM_MOVE:
		if (app)
		{
			//app->OnWindowMoved();
		}
		break;

	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
		{
			if (!s_minimized)
			{
				s_minimized = true;
				if (!s_in_suspend && app)
					//app->OnSuspending();
					s_in_suspend = true;
			}
		}
		else if (s_minimized)
		{
			s_minimized = false;
			if (s_in_suspend && app)
				//app->OnResuming();
				s_in_suspend = false;
		}
		else if (!s_in_sizemove && app)
		{
			//app->OnWindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
		}
		break;

	case WM_ENTERSIZEMOVE:
		s_in_sizemove = true;
		break;

	case WM_EXITSIZEMOVE:
		s_in_sizemove = false;
		if (app)
		{
			RECT rc;
			GetClientRect(hWnd, &rc);

			//app->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
		}
		break;

	case WM_GETMINMAXINFO:
	{
		auto info = reinterpret_cast<MINMAXINFO*>(lParam);
		info->ptMinTrackSize.x = 320;
		info->ptMinTrackSize.y = 200;
	}
	break;

	case WM_ACTIVATEAPP:
		if (app)
		{
			if (wParam)
			{
				//app->OnActivated();
			}
			else
			{
				//app->OnDeactivated();
			}
		}
		break;

	case WM_POWERBROADCAST:
		switch (wParam)
		{
		case PBT_APMQUERYSUSPEND:
			if (!s_in_suspend && app)
				//app->OnSuspending();
				s_in_suspend = true;
			return TRUE;

		case PBT_APMRESUMESUSPEND:
			if (!s_minimized)
			{
				if (s_in_suspend && app)
					//app->OnResuming();
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

				uint32 width = 800;
				uint32 height = 600;
				//if (app)
					//app->GetDefaultSize(width, height);

				ShowWindow(hWnd, SW_SHOWNORMAL);

				SetWindowPos(hWnd, HWND_TOP, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
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
		//app->OnKeyDown(wParam);
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
		//app->OnKeyUp(wParam);
		break;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		//app->OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;

	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		//app->OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;

	case WM_MOUSEMOVE:
		//app->OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;

	case WM_MOUSEWHEEL:
		// x, y is not the coordinate of the cursor but pointer.
		//app->OnMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam), GET_KEYSTATE_WPARAM(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;

	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

// Class Window.
Window::Window()
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

		_bexit_log(!RegisterClassExW(&wcex), "Register Class Failed When Create Window!");

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

		_bexit_log(!m_pHwnd, "Create Window Failed!");

		// TODO: Change nCmdShow to SW_SHOWMAXIMIZED to default to fullscreen.

		//SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(m_app.get()));

		//GetClientRect(m_pHwnd, &rc);
	}
}

Window::Window(const WindowDesc& InWindoWDesc)
	: m_defaultDesc(InWindoWDesc)
{

}

Window::~Window()
{

}

void* Window::GetHinstance() const
{
	return m_pHinstance;
}

void* Window::GetHwnd() const
{
	return m_pHwnd;
}

Window::WindowDesc Window::GetWindowDesc() const
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
			//m_app->Tick();
		}
	}
}

#endif