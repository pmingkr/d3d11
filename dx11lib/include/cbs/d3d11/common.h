#pragma once

#include "../dllconfig.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <assert.h>

#pragma warning(push)
#pragma warning(disable:4005)
#pragma warning(disable:4838)
#include <DXGI.h>
#include <D3DX11.h>
#include <D3D11.h>
#include <xnamath.h>

#pragma warning(disable:4819)
#include <assimp/vector3.h>
#pragma warning(pop)

#include <stdexcept>
#include "../autoutil.h"

namespace cbs
{
	class DuplicationException{};
	class FileException {};

	class DXException
	{
	public:
		const HRESULT hr;
		const wchar_t * const lineText;
		const wchar_t * const filename;
		const int line;

		inline DXException(HRESULT hr)
			:hr(hr), lineText(nullptr), filename(nullptr), line(0)
		{
		}
		inline DXException(HRESULT hr, const wchar_t * lineText, const wchar_t * filename, int line)
			: hr(hr), lineText(lineText), filename(filename), line(line)
		{
		}
	};

	CBS_DX11LIB_EXPORT extern HINSTANCE g_hInst;

	template <typename T> inline void SafeRelease(T * & ptr)
	{
		if(ptr == nullptr) return;
		ptr->Release();
		ptr = nullptr;
	}
	template <typename T, size_t sz> inline void SafeReleaseArray(T * (&arr)[sz])
	{
		for (size_t i = 0; i < sz; i++)
		{
			SafeRelease(arr[i]);
		}
	}

	class vec
	{
	private:
		__m128 m_vector;

	public:
		inline vec()
		{
		}
		inline explicit vec(float x)
		{
			m_vector.m128_f32[0] = x;
			m_vector.m128_f32[1] = x;
			m_vector.m128_f32[2] = x;
			m_vector.m128_f32[3] = 1.f;
		}
		inline vec(float x, float y, float z, float w = 1.f)
		{
			m_vector.m128_f32[0] = x;
			m_vector.m128_f32[1] = y;
			m_vector.m128_f32[2] = z;
			m_vector.m128_f32[3] = w;
		}

		inline operator const XMVECTOR&() const
		{
			return m_vector;
		}
		inline operator const aiVector3D&() const
		{
			return (aiVector3D&)m_vector;
		}
	};
}

#define __WIDE2(x) L##x
#define __WIDE(x) __WIDE2(x)

#define throwhr(cmd)	{HRESULT __hr; if(FAILED(__hr = (cmd))) throw DXException(__hr, __WIDE(#cmd), __WIDE(__FILE__), __LINE__); }
