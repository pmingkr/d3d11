#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "dllconfig.h"

namespace cbs
{
	CBS_DX11LIB_EXPORT extern HINSTANCE g_hInst;

	class WindowException {};

	class Window
	{
	private:
		CBS_DX11LIB_EXPORT static LRESULT CALLBACK _WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		HWND m_hWnd;

	public:
		inline Window(int width, int height)
		{
			WNDCLASSEX wcex;
			memset(&wcex, 0, sizeof(wcex));
			wcex.cbSize = sizeof(wcex);
			wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
			wcex.hInstance = g_hInst;
			wcex.lpfnWndProc = _WndProc;
			wcex.lpszClassName = TEXT("DX11PROJ");
			wcex.style = CS_HREDRAW | CS_VREDRAW;
			RegisterClassEx(&wcex);

			int wndstyle = WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

			RECT rc = { 0, 0, width, height };
			AdjustWindowRect(&rc, wndstyle, false);

			int adjwidth = rc.right - rc.left;
			int adjheight = rc.bottom - rc.top;
			int adjx = (GetSystemMetrics(SM_CXSCREEN) - adjwidth) / 2;
			int adjy = (GetSystemMetrics(SM_CYSCREEN) - adjheight) / 2;

			m_hWnd = CreateWindow(wcex.lpszClassName, TEXT("Direct3D 11 Window"), wndstyle, adjx, adjy, adjwidth, adjheight, nullptr, nullptr, g_hInst, this);
			if (m_hWnd == nullptr) throw WindowException();
		}
		inline ~Window()
		{
		}
		inline int messageLoop()
		{
			MSG msg;
			do
			{
				if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else loop();
			}
			while (msg.message != WM_QUIT);
			return (int)msg.wParam;
		}
		virtual void loop() {}
		virtual void procedure(UINT msg, WPARAM wParam, LPARAM lParam) {}

		inline HWND getWindowHandle()
		{
			return m_hWnd;
		}
	};
}
