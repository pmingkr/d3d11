#include "buffer.h"
#include "device.h"

using namespace cbs;

Buffer::Buffer()
{
}
Buffer::Buffer(UINT bindflags, size_t size)
{
	_create(bindflags, size);
}
Buffer::Buffer(UINT bindflags, size_t size, const void * initdata)
{
	_create(bindflags, size, initdata);
}

void Buffer::_create(UINT bindflags, size_t size)
{
	D3D11_BUFFER_DESC bd;
	memset(&bd, 0, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;
	assert(size <= 0xffffffff);
	bd.ByteWidth = (UINT)size;
	bd.BindFlags = bindflags;
	throwhr(g_device->CreateBuffer(&bd, nullptr, &m_buffer));
}
void Buffer::_create(UINT bindflags, size_t size, const void * initdata)
{
	D3D11_SUBRESOURCE_DATA sd;
	memset(&sd, 0, sizeof(sd));
	sd.pSysMem = initdata;

	D3D11_BUFFER_DESC bd;
	memset(&bd, 0, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;
	assert(size <= 0xffffffff);
	bd.ByteWidth = (UINT)size;
	bd.BindFlags = bindflags;

	throwhr(g_device->CreateBuffer(&bd, &sd, &m_buffer));
}

ID3D11Buffer** Buffer::operator &()
{
	return &m_buffer;
}
Buffer::operator ID3D11Buffer *()
{
	return m_buffer;
}
ID3D11Buffer * Buffer::operator ->()
{
	return m_buffer;
}