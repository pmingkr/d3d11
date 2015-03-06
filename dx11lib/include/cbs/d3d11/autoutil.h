#pragma once

#include "common.h"
#include <new>

namespace cbs
{
	template <typename T> struct GetFirstTemplateParameter;
	template <typename C, template<typename> class T> struct GetFirstTemplateParameter<T<C>>
	{
		typedef C type;
	};

	template <typename T> class Pointer
	{
	protected:
		T * m_ptr;

	public:
		typedef T Component;

		operator T*() const
		{
			return m_ptr;
		}
		T& operator *() const
		{
			return *m_ptr;
		}
		T* operator ->() const
		{
			return m_ptr;
		}
		T* const* operator &() const
		{
			return &m_ptr;
		}
		T** operator &()
		{
			return &m_ptr;
		}

	};


//#define INHERIT_POINTER(...)	\
//		typedef typename GetFirstTemplateParameter<__VA_ARGS__>::type Component;\
//		using __VA_ARGS__::operator &; \
//		using __VA_ARGS__::operator Component *; \
//		using __VA_ARGS__::operator ->;

	template <typename T> class AutoRelease:public Pointer<T>
	{
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
		AutoRelease(AutoRelease &&move)
		{
			m_ptr = move.m_ptr;
			move.m_ptr = nullptr;
		}
		~AutoRelease()
		{
			if (m_ptr != nullptr) m_ptr->Release();
		}

		void move(T* ptr)
		{
			m_ptr = ptr;
		}
		AutoRelease& operator =(const AutoRelease& copy)
		{
			this->~AutoRelease();
			new(this) AutoRelease(copy);
			return *this;
		}
		AutoRelease& operator =(AutoRelease&& move)
		{
			this->~AutoRelease();
			new(this) AutoRelease(std::move(move));
			return *this;
		}
		template <typename T>
		AutoRelease<T> as()
		{
			AutoRelease<T> t;
			if(FAILED(m_ptr->QueryInterface(__uuidof(T), &t))) return nullptr;
			return t;
		}
	};

	template <typename T> class AutoDelete :public Pointer<T>
	{
	public:
		AutoDelete()
		{
			m_ptr = nullptr;
		}
		AutoDelete(T * ptr)
		{
			m_ptr = ptr;
		}
		AutoDelete(AutoDelete && move)
		{
			m_ptr = move.m_ptr;
			move.m_ptr = nullptr;
		}
		~AutoDelete()
		{
			delete m_ptr;
		}

		AutoDelete& operator =(AutoDelete&& move)
		{
			this->~AutoDelete();
			new(this) AutoDelete(move);
			return *this;
		}

	private:
		AutoDelete(const AutoDelete& copy ); // = delete;
		AutoDelete& operator =(const AutoDelete& copy); // = delete
	};

	template <typename T> class AutoDeleteArray :public Pointer<T>
	{
	public:
		AutoDeleteArray()
		{
			m_ptr = nullptr;
		}
		AutoDeleteArray(T * ptr)
		{
			m_ptr = ptr;
		}
		AutoDeleteArray(AutoDeleteArray && move)
		{
			m_ptr = move.m_ptr;
			move.m_ptr = nullptr;
		}
		~AutoDeleteArray()
		{
			delete [] m_ptr;
		}

		void allocate(size_t count)
		{
			delete [] m_ptr;
			m_ptr = new T[count];
		}

		AutoDeleteArray& operator =(AutoDeleteArray&& move)
		{
			this->~AutoDeleteArray();
			new(this) AutoDeleteArray(std::move(move));
			return *this;
		}

	private:
		AutoDeleteArray(const AutoDeleteArray& copy); // = delete;
		AutoDeleteArray& operator =(const AutoDeleteArray& copy); // = delete
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
		AutoClose(AutoClose && move)
		{
			m_ptr = move.m_ptr;
			move.m_ptr = nullptr;
		}
		~AutoClose()
		{
			if (m_ptr != nullptr && m_ptr != INVALID_HANDLE_VALUE)
				CloseHandle(m_ptr);
		}
		operator T() const
		{
			return m_ptr;
		}
		T* operator &() const
		{
			return &m_ptr;
		}

		AutoClose& operator =(AutoClose&& move)
		{
			this->~AutoClose();
			new(this) AutoClose(move);
			return *this;
		}
	private:
		AutoClose(const AutoClose& copy); // = delete;
		AutoClose& operator =(const AutoClose& copy); // = delete
	};
}
