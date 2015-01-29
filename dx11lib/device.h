#pragma once

#include "common.h"
#include "win.h"

namespace cbs
{
	extern AutoRelease<ID3D11Device>			g_device;
	extern AutoRelease<ID3D11DeviceContext>		g_context;
	extern AutoRelease<IDXGISwapChain>			g_chain;
	extern AutoRelease<ID3D11RenderTargetView>	g_rtv;

	class D3D11Device :public Window
	{
	public:
		D3D11Device(int width, int height);
		~D3D11Device();
	};
}