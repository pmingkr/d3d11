#include "shader.h"
#include "device.h"
using namespace cbs;

namespace
{

	template <typename T> void LoadShader(const wchar_t * strFileName, const char * version, T lambda)
	{
		AutoRelease<ID3D10Blob> blob;
		AutoRelease<ID3D10Blob> bloberr;
		HRESULT hr = D3DX11CompileFromFile(strFileName, nullptr, nullptr, "main", version, 0, 0, nullptr, &blob, &bloberr, nullptr);

		if (bloberr != nullptr)
		{
			char *errors = (char*)bloberr->GetBufferPointer();
			OutputDebugStringA(errors);
			MessageBoxA(nullptr, errors, nullptr, MB_OK | MB_ICONERROR);
		}
		if (FAILED(hr)) throw cbs::DXException(hr);

		lambda(blob->GetBufferPointer(), blob->GetBufferSize());
	}

}
cbs::VertexShader::VertexShader()
{
}
cbs::VertexShader::VertexShader(LPCTSTR strFileName, const D3D11_INPUT_ELEMENT_DESC * desc, size_t count)
{
	_create(strFileName, desc, count);
}
cbs::VertexShader::VertexShader(const BYTE * data, size_t size, const D3D11_INPUT_ELEMENT_DESC * desc, size_t count)
{
	_create(data, size, desc, count);
}
ID3D11InputLayout *cbs::VertexShader::getInputLayer()
{
	return m_layout;
}
void cbs::VertexShader::_create(LPCTSTR strFileName, const D3D11_INPUT_ELEMENT_DESC * desc, size_t count)
{
	assert(count <= 0xffffffff);
	LoadShader(strFileName, "vs_4_0", [&](void * data, size_t size)
	{
		_create((const BYTE *)data, size, desc, count);
	});
}
void cbs::VertexShader::_create(const BYTE * data, size_t sz, const D3D11_INPUT_ELEMENT_DESC * desc, size_t count)
{
	assert(count <= 0xffffffff);

	throwhr(g_device->CreateVertexShader(data, sz, nullptr, &m_ptr));
	throwhr(g_device->CreateInputLayout(desc, (UINT)count, data, sz, &m_layout));
}


cbs::PixelShader::PixelShader()
{

}
cbs::PixelShader::PixelShader(LPCTSTR strFileName)
{
	LoadShader(strFileName, "ps_4_0", [&](void * data, size_t size)
	{
		_create((const BYTE *)data, size);
	});
}
cbs::PixelShader::PixelShader(const BYTE * data, size_t size)
{
	_create(data, size);
}
void cbs::PixelShader::_create(const BYTE * data, size_t sz)
{
	throwhr(g_device->CreatePixelShader(data, sz, nullptr, &m_ptr));
}