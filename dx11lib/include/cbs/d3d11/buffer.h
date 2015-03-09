#pragma once

#include "common.h"

namespace cbs
{
	class Buffer: public AutoRelease<ID3D11Buffer>
	{
	public:
		CBS_DX11LIB_EXPORT Buffer();
		CBS_DX11LIB_EXPORT Buffer(UINT bindflags, size_t size);
		CBS_DX11LIB_EXPORT Buffer(UINT bindflags, size_t size, const void * initdata);
				
		template <typename T, size_t size>
		inline Buffer(UINT bindflags, const T(&initdata)[size])
		{
			_create(D3D11_USAGE_DEFAULT, (D3D11_CPU_ACCESS_FLAG)0, bindflags, size * sizeof(T), initdata);
		}

	protected:
		void _create(D3D11_USAGE usage, D3D11_CPU_ACCESS_FLAG cpuflags, UINT bindflags, size_t size);
		void _create(D3D11_USAGE usage, D3D11_CPU_ACCESS_FLAG cpuflags, UINT bindflags, size_t size, const void * initdata);
	};

	class DynamicBuffer: public Buffer
	{
	public:
		CBS_DX11LIB_EXPORT DynamicBuffer();
		CBS_DX11LIB_EXPORT DynamicBuffer(UINT bindflags, size_t size);
		CBS_DX11LIB_EXPORT DynamicBuffer(UINT bindflags, size_t size, const void * initdata);

		CBS_DX11LIB_EXPORT void* map();
		CBS_DX11LIB_EXPORT void unmap();

		using Buffer::operator &;
		using Buffer::operator ID3D11Buffer *;
		using Buffer::operator ->;

		template <typename T, size_t size>
		inline DynamicBuffer(UINT bindflags, const T(&initdata)[size])
		{
			_create(bindflags, size * sizeof(T), initdata);
		}
	};
}