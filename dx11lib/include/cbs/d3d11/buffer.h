#pragma once

#include "common.h"
#include "device.h"
#include "../datalist.h"
#include <new>

namespace cbs
{
	class Buffer: public AutoRelease<ID3D11Buffer>
	{
	protected:
		inline void _create(D3D11_USAGE usage, D3D11_CPU_ACCESS_FLAG cpuflags, UINT bindflags, size_t size)
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
		inline void _create(D3D11_USAGE usage, D3D11_CPU_ACCESS_FLAG cpuflags, UINT bindflags, AnyData initdata)
		{
			D3D11_SUBRESOURCE_DATA sd;
			memset(&sd, 0, sizeof(sd));
			sd.pSysMem = initdata.begin();

			D3D11_BUFFER_DESC bd;
			memset(&bd, 0, sizeof(bd));
			bd.Usage = usage;
			bd.CPUAccessFlags = cpuflags;
			assert(initdata.size() <= 0xffffffff);
			bd.ByteWidth = (UINT)initdata.size();
			bd.BindFlags = bindflags;

			throwhr(g_device->CreateBuffer(&bd, &sd, &m_ptr));
		}

	public:
		inline Buffer()
		{
		}
		inline Buffer(UINT bindflags, size_t size)
		{
			_create(D3D11_USAGE_DEFAULT, (D3D11_CPU_ACCESS_FLAG)0, bindflags, size);
		}
		inline Buffer(UINT bindflags, AnyData initdata)
		{
			_create(D3D11_USAGE_DEFAULT, (D3D11_CPU_ACCESS_FLAG)0, bindflags, initdata);
		}
	};

	class DynamicBuffer: public Buffer
	{
	public:
		inline DynamicBuffer()
		{
		}
		inline DynamicBuffer(UINT bindflags, size_t size)
		{
			_create(D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, bindflags, size);
		}
		inline DynamicBuffer(UINT bindflags, AnyData initdata)
		{
			_create(D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, bindflags, initdata);
		}

		inline void* map()
		{
			D3D11_MAPPED_SUBRESOURCE res;
			throwhr(g_context->Map(m_ptr, 0, D3D11_MAP_WRITE_DISCARD, 0, &res));
			return res.pData;
		}
		inline void unmap()
		{
			g_context->Unmap(m_ptr, 0);
		}

		using Buffer::operator &;
		using Buffer::operator ID3D11Buffer *;
		using Buffer::operator ->;
	};
}