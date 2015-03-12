#pragma once

#include "common.h"
#include "../win.h"

namespace cbs
{
	CBS_DX11LIB_EXPORT extern AutoRelease<ID3D11Device>				g_device;
	CBS_DX11LIB_EXPORT extern AutoRelease<ID3D11DeviceContext>		g_context;
	CBS_DX11LIB_EXPORT extern AutoRelease<IDXGISwapChain>			g_chain;
	CBS_DX11LIB_EXPORT extern AutoRelease<ID3D11RenderTargetView>	g_rtv;
	CBS_DX11LIB_EXPORT extern AutoRelease<ID3D11DepthStencilView>	g_dsv;

	class D3D11Device :public Window
	{
	public:
		CBS_DX11LIB_EXPORT D3D11Device(int width, int height, int multiSampling = 1);
		CBS_DX11LIB_EXPORT ~D3D11Device();

		inline static void clear()
		{
			// 색 버퍼 지우기
			float colors[4] = { 0,0,0,1 };
			g_context->ClearRenderTargetView(g_rtv, colors);
			g_context->ClearDepthStencilView(g_dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
		}
	};
}