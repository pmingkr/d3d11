#pragma once

#include "global.h"
#include "win.h"

#pragma warning(push)
#pragma warning(disable:4005)
#pragma warning(disable:4838)
#include <D3DX11.h>
#include <D3D11.h>
#include <xnamath.h>
#pragma warning(pop)

#include <stdexcept>
#include "autoutil.h"

class FileException {};

class DXException
{
public:
	DXException(HRESULT hr);
	operator HRESULT();

private:
	HRESULT m_hr;
};

#define throwhr(cmd)	{HRESULT hr; if(FAILED(hr = (cmd))) throw DXException(hr); }

class D3D11Device:public Window
{
public:
	D3D11Device(int width, int height);
	~D3D11Device();

	AutoRelease<ID3D11VertexShader> LoadVertexShader(LPCTSTR strFileName, ID3D11InputLayout **ppLayout,
		const D3D11_INPUT_ELEMENT_DESC * desc, size_t count);

	template <size_t sz>
	AutoRelease<ID3D11VertexShader> LoadVertexShader(LPCTSTR strFileName, ID3D11InputLayout **ppLayout,
		const D3D11_INPUT_ELEMENT_DESC (&desc)[sz])
	{
		return LoadVertexShader(strFileName, ppLayout, desc, sz);
	}
	AutoRelease<ID3D11PixelShader> LoadPixelShader(LPCTSTR strFileName);

protected:
	AutoRelease<ID3D11Device>			m_device;
	AutoRelease<ID3D11DeviceContext>	m_context;
	AutoRelease<IDXGISwapChain>			m_chain;
	AutoRelease<ID3D11RenderTargetView> m_rtv;
};

class Buffer
{
public:
	Buffer();
	Buffer(ID3D11Device * device, UINT bindflags, size_t size);
	Buffer(ID3D11Device * device, UINT bindflags, size_t size, const void * initdata);

	void allocate(ID3D11Device * device, UINT bindflags, size_t size);
	void allocate(ID3D11Device * device, UINT bindflags, size_t size, const void * initdata);

	ID3D11Buffer** operator &();
	operator ID3D11Buffer *();
	ID3D11Buffer * operator ->();

	template <typename T, size_t size>
	Buffer(ID3D11Device * device, UINT bindflags, const T(&initdata)[size])
	{
		allocate(device, bindflags, size * sizeof(T), initdata);
	}
	
private:
	AutoRelease<ID3D11Buffer> m_buffer;
};