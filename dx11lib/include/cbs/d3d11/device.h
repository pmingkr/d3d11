#pragma once

#include "common.h"
#include "win.h"

namespace cbs
{
	extern AutoRelease<ID3D11Device>			g_device;
	extern AutoRelease<ID3D11DeviceContext>		g_context;
	extern AutoRelease<IDXGISwapChain>			g_chain;
	extern AutoRelease<ID3D11RenderTargetView>	g_rtv;
	extern AutoRelease<ID3D11DepthStencilView>	g_dsv;

	class D3D11Device :public Window
	{
	public:
		D3D11Device(int width, int height, int multiSampling = 1);
		~D3D11Device();

		static void clear();
	};
}