#pragma once

#include "common.h"

namespace cbs
{
	class VertexShader: public AutoRelease<ID3D11VertexShader>
	{
	public:
		CBS_DX11LIB_EXPORT VertexShader();
		CBS_DX11LIB_EXPORT VertexShader(const BYTE * data, size_t sz, const D3D11_INPUT_ELEMENT_DESC * desc, size_t count);

		template <size_t sz, size_t iesz>
		inline VertexShader(const BYTE(&data)[sz], const D3D11_INPUT_ELEMENT_DESC(&desc)[iesz])
		{
			_create(data, sz, desc, iesz);
		}

		CBS_DX11LIB_EXPORT ID3D11InputLayout * getInputLayer();

	private:
		CBS_DX11LIB_EXPORT void _create(const BYTE * data, size_t sz, const D3D11_INPUT_ELEMENT_DESC * desc, size_t count);

		AutoRelease<ID3D11InputLayout> m_layout;
	};
	class PixelShader : public AutoRelease<ID3D11PixelShader>
	{
	public:
		CBS_DX11LIB_EXPORT PixelShader();
		CBS_DX11LIB_EXPORT explicit PixelShader(const BYTE * data, size_t sz);

		template <size_t sz>
		inline explicit PixelShader(const BYTE(&data)[sz])
		{
			_create(data, sz);
		}

	private:
		CBS_DX11LIB_EXPORT void _create(const BYTE * data, size_t sz);
	};

}