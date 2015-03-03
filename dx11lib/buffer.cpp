#include "include/cbs/d3d11/buffer.h"
#include "include/cbs/d3d11/device.h"

using namespace cbs;

Buffer::Buffer()
{
}
Buffer::Buffer(UINT bindflags, size_t size)
{
	_create(D3D11_USAGE_DEFAULT, (D3D11_CPU_ACCESS_FLAG)0, bindflags, size);
}
Buffer::Buffer(UINT bindflags, size_t size, const void * initdata)
{
	_create(D3D11_USAGE_DEFAULT, (D3D11_CPU_ACCESS_FLAG)0, bindflags, size, initdata);
}

void Buffer::_create(D3D11_USAGE usage, D3D11_CPU_ACCESS_FLAG cpuflags, UINT bindflags, size_t size)
{
	D3D11_BUFFER_DESC bd;
	memset(&bd, 0, sizeof(bd));
	bd.Usage = usage;
	bd.CPUAccessFlags = cpuflags;
	assert(size <= 0xffffffff);
	bd.ByteWidth = (UINT)size;
	bd.BindFlags = bindflags;
	throwhr(g_device->CreateBuffer(&bd, nullptr, &m_ptr));
}
void Buffer::_create(D3D11_USAGE usage, D3D11_CPU_ACCESS_FLAG cpuflags, UINT bindflags, size_t size, const void * initdata)
{
	D3D11_SUBRESOURCE_DATA sd;
	memset(&sd, 0, sizeof(sd));
	sd.pSysMem = initdata;

	D3D11_BUFFER_DESC bd;
	memset(&bd, 0, sizeof(bd));
	bd.Usage = usage;
	bd.CPUAccessFlags = cpuflags;
	assert(size <= 0xffffffff);
	bd.ByteWidth = (UINT)size;
	bd.BindFlags = bindflags;

	throwhr(g_device->CreateBuffer(&bd, &sd, &m_ptr));
}

DynamicBuffer::DynamicBuffer()
{
}
DynamicBuffer::DynamicBuffer(UINT bindflags, size_t size)
{
	_create(D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, bindflags, size);
}
DynamicBuffer::DynamicBuffer(UINT bindflags, size_t size, const void * initdata)
{
	_create(D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, bindflags, size, initdata);
}
void* DynamicBuffer::map()
{
	D3D11_MAPPED_SUBRESOURCE res;
	throwhr(g_context->Map(m_ptr, 0, D3D11_MAP_WRITE_DISCARD, 0, &res));
	return res.pData;
}
void DynamicBuffer::unmap()
{
	g_context->Unmap(m_ptr, 0);
}
