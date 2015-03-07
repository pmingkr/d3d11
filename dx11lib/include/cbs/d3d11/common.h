#pragma once

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
		DXException(HRESULT hr);
		DXException(HRESULT hr, const wchar_t * lineText, const wchar_t * filename, int line);

		const HRESULT hr;
		const wchar_t * const lineText;
		const wchar_t * const filename;
		const int line;
	};

	extern HINSTANCE g_hInst;

	template <typename T> void SafeRelease(T * & ptr)
	{
		if(ptr == nullptr) return;
		ptr->Release();
		ptr = nullptr;
	}
	template <typename T, size_t sz> void SafeReleaseArray(T * (&arr)[sz])
	{
		for (size_t i = 0; i < sz; i++)
		{
			SafeRelease(arr[i]);
		}
	}

	class vec
	{
	public:
		vec();
		explicit vec(float x);
		vec(float x, float y, float z, float w = 1.f);

		operator const XMVECTOR&() const;
		operator const aiVector3D&() const;

	private:
		__m128 m_vector;
	};
}

#define __WIDE2(x) L##x
#define __WIDE(x) __WIDE2(x)

#define throwhr(cmd)	{HRESULT __hr; if(FAILED(__hr = (cmd))) throw DXException(__hr, __WIDE(#cmd), __WIDE(__FILE__), __LINE__); }
