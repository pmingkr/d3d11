#pragma once

#include "common.h"


namespace cbs
{
	class SamplerState:public Pointer<ID3D11SamplerState>
	{
	public:
		CBS_DX11LIB_EXPORT SamplerState();
		CBS_DX11LIB_EXPORT SamplerState(D3D11_TEXTURE_ADDRESS_MODE ta, D3D11_FILTER filter);
	};

	class RasterizerState:public Pointer<ID3D11RasterizerState>
	{
	public:
		CBS_DX11LIB_EXPORT RasterizerState();
		CBS_DX11LIB_EXPORT RasterizerState(D3D11_CULL_MODE cull, D3D11_FILL_MODE fill, bool depthClipping);
	};

	class DepthStencilState : public Pointer<ID3D11DepthStencilState>
	{
	public:
		CBS_DX11LIB_EXPORT DepthStencilState();
		CBS_DX11LIB_EXPORT DepthStencilState(D3D11_COMPARISON_FUNC cmp, D3D11_DEPTH_WRITE_MASK mask);
	};
	
	template <typename T> struct StateConst;
	template <> struct StateConst<D3D11_TEXTURE_ADDRESS_MODE> { enum { count = 5, offset = 1 }; };
	template <> struct StateConst<D3D11_FILTER> { enum { count = 9, offset = -1 }; };
	template <> struct StateConst<D3D11_CULL_MODE> { enum { count = 3, offset = 1 }; };
	template <> struct StateConst<D3D11_FILL_MODE> { enum { count = 2, offset = 2 }; };
	template <> struct StateConst<D3D11_COMPARISON_FUNC> { enum { count = 8, offset = 1 }; };
	template <> struct StateConst<D3D11_DEPTH_WRITE_MASK> { enum { count = 2, offset = 0 }; };
	template <> struct StateConst<bool> { enum { count = 2, offset = 0 }; };
	

	class StateContainer
	{
	public:
		static void clear();
	
	private:
		friend SamplerState;
		friend RasterizerState;
		friend DepthStencilState;

		ID3D11SamplerState *samplers[
			StateConst<D3D11_TEXTURE_ADDRESS_MODE>::count * StateConst<D3D11_FILTER>::count
		];
		ID3D11RasterizerState *rasterizer[
			StateConst<D3D11_CULL_MODE>::count * StateConst<D3D11_FILL_MODE>::count
		];
		ID3D11DepthStencilState *depthStencil[
			StateConst<D3D11_COMPARISON_FUNC>::count * StateConst<D3D11_DEPTH_WRITE_MASK>::count + 1
		];

		static StateContainer m_container;
	};

}