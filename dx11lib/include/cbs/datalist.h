#pragma once

#if _MSC_VER > 1800 // 이니셜라이저 리스트 / VS 2013 이상
#include <initializer_list>
#endif

namespace cbs
{
	
	template <typename T> class DataList
	{
	private:
		const T * m_begin;
		size_t m_size;

	public:
		inline DataList()
		{
			m_begin = nullptr;
			m_size = 0;
		}
		inline DataList(const T * begin, size_t size)
		{
			m_begin = begin;
			m_size = size;
		}
		template <size_t size>
		inline DataList(const T(&arr)[size])
		{
			m_begin = arr;
			m_size = size;
		}
#if _MSC_VER > 1800 // 이니셜라이저 리스트 / VS 2013 이상
		inline DataList(std::initializer_list<T> il)
		{
			m_begin = il.begin();
			m_size = il.size();
		}
#endif

		inline const T * begin() const
		{
			return m_begin;
		}
		inline const T * end() const
		{
			return m_begin + m_size;
		}
		inline size_t size() const
		{
			return m_size;
		}
		inline operator const T*() const
		{
			return m_begin;
		}
	};

	template <> class DataList<void>
	{
	private:
		const void * m_begin;
		size_t m_size;

	public:
		inline DataList()
		{
			m_begin = nullptr;
			m_size = 0;
		}
		inline DataList(const void * begin, size_t size)
		{
			m_begin = begin;
			m_size = size;
		}
		template <typename T, size_t size>
		inline DataList(const T(&arr)[size])
		{
			m_begin = arr;
			m_size = size * sizeof(T);
		}
#if _MSC_VER > 1800 // 이니셜라이저 리스트 / VS 2013 이상
		template <typename T>
		inline DataList(std::initializer_list<T> il)
		{
			m_begin = il.begin();
			m_size = il.size() * sizeof(T);
		}
#endif

		inline const void * begin() const
		{
			return m_begin;
		}
		inline const void * end() const
		{
			return (char*)m_begin + m_size;
		}
		inline size_t size() const
		{
			return m_size;
		}
		inline operator const void*() const
		{
			return m_begin;
		}
	};

	typedef DataList<void> AnyData;
}