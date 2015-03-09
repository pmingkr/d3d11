#include "include/cbs/d3d11/shader.h"
#include "include/cbs/d3d11/device.h"
using namespace cbs;

cbs::VertexShader::VertexShader()
{
}
cbs::VertexShader::VertexShader(const BYTE * data, size_t size, const D3D11_INPUT_ELEMENT_DESC * desc, size_t count)
{
	_create(data, size, desc, count);
}
ID3D11InputLayout *cbs::VertexShader::getInputLayer()
{
	return m_layout;
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
cbs::PixelShader::PixelShader(const BYTE * data, size_t size)
{
	_create(data, size);
}
void cbs::PixelShader::_create(const BYTE * data, size_t sz)
{
	throwhr(g_device->CreatePixelShader(data, sz, nullptr, &m_ptr));
}