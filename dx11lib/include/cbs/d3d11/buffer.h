#pragma once

#include "common.h"

namespace cbs
{
	class Buffer: public AutoRelease<ID3D11Buffer>
	{
	public:
		Buffer();
		Buffer(UINT bindflags, size_t size);
		Buffer(UINT bindflags, size_t size, const void * initdata);

		
		template <typename T, size_t size>
		Buffer(UINT bindflags, const T(&initdata)[size])
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
		DynamicBuffer();
		DynamicBuffer(UINT bindflags, size_t size);
		DynamicBuffer(UINT bindflags, size_t size, const void * initdata);

		void* map();
		void unmap();

		using Buffer::operator &;
		using Buffer::operator ID3D11Buffer *;
		using Buffer::operator ->;

		template <typename T, size_t size>
		DynamicBuffer(UINT bindflags, const T(&initdata)[size])
		{
			_create(bindflags, size * sizeof(T), initdata);
		}
	};
}