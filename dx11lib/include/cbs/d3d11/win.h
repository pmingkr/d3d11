#pragma once

#include "common.h"

namespace cbs
{
	class WindowException {};

	class Window
	{
	public:
		Window(int width, int height);
		~Window();
		int messageLoop();
		virtual void loop();
		virtual void procedure(UINT msg, WPARAM wParam, LPARAM lParam);

		HWND getWindowHandle();

	private:
		static LRESULT CALLBACK _WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		HWND m_hWnd;
	};
}
