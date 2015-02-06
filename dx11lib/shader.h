#pragma once

#include "common.h"

namespace cbs
{
	class VertexShader: public AutoRelease<ID3D11VertexShader>
	{
	public:
		VertexShader();
		VertexShader(LPCTSTR strFileName, const D3D11_INPUT_ELEMENT_DESC * desc, size_t count);
		VertexShader(const BYTE * data, size_t sz, const D3D11_INPUT_ELEMENT_DESC * desc, size_t count);

		template <size_t sz>
		VertexShader(LPCTSTR strFileName, const D3D11_INPUT_ELEMENT_DESC(&desc)[sz])
		{
			_create(strFileName, desc, sz);
		}
		template <size_t sz, size_t iesz>
		VertexShader(const BYTE(&data)[sz], const D3D11_INPUT_ELEMENT_DESC(&desc)[iesz])
		{
			_create(data, sz, desc, iesz);
		}

		ID3D11InputLayout * getInputLayer();

		using AutoRelease::operator ->;
		using AutoRelease::operator &;

	private:
		void _create(LPCTSTR strFileName, const D3D11_INPUT_ELEMENT_DESC * desc, size_t count);
		void _create(const BYTE * data, size_t sz, const D3D11_INPUT_ELEMENT_DESC * desc, size_t count);

		AutoRelease<ID3D11InputLayout> m_layout;
	};
	class PixelShader : public AutoRelease<ID3D11PixelShader>
	{
	public:
		PixelShader();
		explicit PixelShader(LPCTSTR strFileName);
		explicit PixelShader(const BYTE * data, size_t sz);

		template <size_t sz>
		explicit PixelShader(const BYTE(&data)[sz])
		{
			_create(data, sz);
		}

		using AutoRelease::operator ->;
		using AutoRelease::operator &;

	private:
		void _create(const BYTE * data, size_t sz);
	};

}