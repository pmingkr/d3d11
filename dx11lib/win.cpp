#include "include/cbs/win.h"

HINSTANCE cbs::g_hInst;

LRESULT CALLBACK cbs::Window::_WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	try
	{
		Window* window = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		switch (msg)
		{
		case WM_NCCREATE:
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)((LPCREATESTRUCT)lParam)->lpCreateParams);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}
		if (window != nullptr) window->procedure(msg,wParam,lParam);
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	catch (LRESULT lr)
	{
		return lr;
	}
}
