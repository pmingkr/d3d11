#include "win.h"


HWND cbs::g_hWnd = nullptr;

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

using namespace cbs;

Window::Window(int width, int height)
{
	if(g_hWnd != nullptr) throw DuplicationException();

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

	int nWidth = GetSystemMetrics(SM_CXDLGFRAME) * 2 + width;
	int nHeight = GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CXDLGFRAME) * 2 + height;
	int nX = (GetSystemMetrics(SM_CXSCREEN) - nWidth) / 2;
	int nY = (GetSystemMetrics(SM_CYSCREEN) - nHeight) / 2;

	g_hWnd = CreateWindow(wcex.lpszClassName, TEXT("Direct3D 11 Window"), WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, nX, nY, nWidth, nHeight, nullptr, nullptr, g_hInst, nullptr);
	if (g_hWnd == nullptr) throw WindowException();
}
Window::~Window()
{
}
int Window::messageLoop()
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
void Window::loop()
{
}
void Window::procedure(UINT msg, WPARAM wParam, LPARAM lParam)
{
}
