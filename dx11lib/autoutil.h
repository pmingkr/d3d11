#pragma once

#include "common.h"

namespace cbs
{

	template <typename T> class AutoRelease
	{
	protected:
		T * m_ptr;

	public:
		AutoRelease()
		{
			m_ptr = nullptr;
		}
		AutoRelease(T * ptr)
		{
			m_ptr = ptr;
			if (ptr != nullptr) ptr->AddRef();
		}
		AutoRelease(const AutoRelease &copy)
		{
			m_ptr = copy.m_ptr;
			if (m_ptr != nullptr) m_ptr->AddRef();
		}
		~AutoRelease()
		{
			if (m_ptr != nullptr) m_ptr->Release();
		}
		operator T*()
		{
			return m_ptr;
		}
		T* operator ->()
		{
			return m_ptr;
		}
		T** operator &()
		{
			return &m_ptr;
		}

		void move(T* ptr)
		{
			m_ptr = ptr;
		}
		AutoRelease& operator =(const AutoRelease& copy)
		{
			if (m_ptr != nullptr) m_ptr->Release();
			m_ptr = copy.m_ptr;
			if (m_ptr != nullptr) m_ptr->AddRef();
			return *this;
		}
	};

	template <typename T> class AutoDelete
	{
	protected:
		T * m_ptr;

	public:
		AutoDelete()
		{
			m_ptr = nullptr;
		}
		AutoDelete(T * ptr)
		{
			m_ptr = ptr;
		}
		~AutoDelete()
		{
			delete m_ptr;
		}
		operator T*()
		{
			return m_ptr;
		}
		T* operator ->()
		{
			return m_ptr;
		}
		T** operator &()
		{
			return &m_ptr;
		}
	};

	template <typename T> class AutoClose
	{
	protected:
		T m_ptr;

	public:
		AutoClose()
		{
		}
		AutoClose(T ptr)
		{
			m_ptr = ptr;
		}
		~AutoClose()
		{
			if (m_ptr != nullptr && m_ptr != INVALID_HANDLE_VALUE)
				CloseHandle(m_ptr);
		}
		operator T()
		{
			return m_ptr;
		}
		T* operator &()
		{
			return &m_ptr;
		}
	};
}
