#pragma once

#include "common.h"

namespace cbs
{
	class WindowException {};

	class Window
	{
	public:
		CBS_DX11LIB_EXPORT Window(int width, int height);
		CBS_DX11LIB_EXPORT ~Window();
		CBS_DX11LIB_EXPORT int messageLoop();
		CBS_DX11LIB_EXPORT virtual void loop();
		CBS_DX11LIB_EXPORT virtual void procedure(UINT msg, WPARAM wParam, LPARAM lParam);

		CBS_DX11LIB_EXPORT HWND getWindowHandle();

	private:
		static LRESULT CALLBACK _WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		HWND m_hWnd;
	};
}
