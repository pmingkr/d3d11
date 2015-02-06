#pragma once

#include "common.h"

namespace cbs
{
	class Buffer
	{
	public:
		Buffer();
		Buffer(UINT bindflags, size_t size);
		Buffer(UINT bindflags, size_t size, const void * initdata);

		ID3D11Buffer** operator &();
		operator ID3D11Buffer *();
		ID3D11Buffer * operator ->();

		template <typename T, size_t size>
		Buffer(UINT bindflags, const T(&initdata)[size])
		{
			_create(bindflags, size * sizeof(T), initdata);
		}

	private:
		void _create(UINT bindflags, size_t size);
		void _create(UINT bindflags, size_t size, const void * initdata);

		AutoRelease<ID3D11Buffer> m_buffer;
	};
}