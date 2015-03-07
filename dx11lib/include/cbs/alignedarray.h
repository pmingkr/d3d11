#pragma once

#include <stddef.h>
#include <malloc.h>
#include <new>

namespace cbs
{
	template <typename T, size_t alignment>
	class AlignedArray
	{
	public:
		AlignedArray();
		~AlignedArray();
		AlignedArray(const AlignedArray& copy);
		AlignedArray(AlignedArray&& move);

		AlignedArray& operator =(const AlignedArray& copy);
		AlignedArray& operator =(AlignedArray&& move);
		T & operator [](size_t i);
		const T & operator [](size_t i) const;
		T * data();
		const T * data() const;

		size_t size() const;
		bool empty() const;

		//void resize();
		void resizeWithoutKeep(size_t sz);

		void remove();

	private:
		void _alloc(size_t sz);

		T * m_data;
		size_t m_capacity;
		size_t m_size;
	};

	template <typename T, size_t alignment>
	AlignedArray<T, alignment>::AlignedArray()
	{
		m_data = nullptr;
		m_capacity = 0;
		m_size = 0;
	}
	template <typename T, size_t alignment>
	AlignedArray<T, alignment>::~AlignedArray()
	{
		remove();
	}
	template <typename T, size_t alignment>
	AlignedArray<T, alignment>::AlignedArray(const AlignedArray& copy)
	{
		m_size = copy.m_size;
		_alloc(m_size);
		memcpy(m_data, copy.m_data, m_size * sizeof(XMMATRIX));
	}
	template <typename T, size_t alignment>
	AlignedArray<T, alignment>::AlignedArray(AlignedArray&& move)
	{
		m_data = move.m_data;
		m_size = move.m_size;
		m_capacity = move.m_capacity;
		move.m_size = 0;
		move.m_data = nullptr;
		move.m_capacity = 0;
	}

	template <typename T, size_t alignment>
	AlignedArray<T, alignment>& AlignedArray<T, alignment>::operator =(const AlignedArray& copy)
	{
		this->~AlignedArray();
		new(this) AlignedArray(copy);
		return *this;
	}
	template <typename T, size_t alignment>
	AlignedArray<T, alignment>& AlignedArray<T, alignment>::operator =(AlignedArray&& move)
	{
		this->~AlignedArray();
		new(this) AlignedArray(std::move(move));
		return *this;
	}

	template <typename T, size_t alignment>
	T & AlignedArray<T, alignment>::operator [](size_t i)
	{
		assert(i < m_size);
		return m_data[i];
	}
	template <typename T, size_t alignment>
	const T & AlignedArray<T, alignment>::operator [](size_t i) const
	{
		assert(i < m_size);
		return m_data[i];
	}
	template <typename T, size_t alignment>
	T * AlignedArray<T, alignment>::data()
	{
		return m_data;
	}
	template <typename T, size_t alignment>
	const T * AlignedArray<T, alignment>::data() const
	{
		return m_data;
	}

	template <typename T, size_t alignment>
	size_t AlignedArray<T, alignment>::size() const
	{
		return m_size;
	}
	template <typename T, size_t alignment>
	bool AlignedArray<T, alignment>::empty() const
	{
		return m_size == 0;
	}
	
	template <typename T, size_t alignment>
	void AlignedArray<T, alignment>::resizeWithoutKeep(size_t sz)
	{
		if (m_capacity >= sz)
		{
			m_size = sz;
			return;
		}

		size_t ncap = m_capacity * 2;
		if (sz > ncap)
		{
			_alloc(sz);
		}
		else
		{
			remove();
			_alloc(ncap);
		}
		m_size = sz;
	}
	template <typename T, size_t alignment>
	void AlignedArray<T, alignment>::remove()
	{
		if (m_data != nullptr)
		{
			_aligned_free(m_data);
			m_data = nullptr;
			m_capacity = 0;
			m_size = 0;
		}
	}
	
	template <typename T, size_t alignment>
	void AlignedArray<T, alignment>::_alloc(size_t sz)
	{
		m_capacity = sz;
		m_data = (T*)_aligned_malloc(sz * sizeof(T), alignment);
	}
}