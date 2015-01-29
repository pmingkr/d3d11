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
#pragma warning(pop)

#include <stdexcept>
#include "autoutil.h"

namespace cbs
{
	class DuplicationException{};
	class FileException {};

	class DXException
	{
	public:
		DXException(HRESULT hr);
		operator HRESULT();

	private:
		HRESULT m_hr;
	};

	extern HINSTANCE g_hInst;
}

#define throwhr(cmd)	{HRESULT hr; if(FAILED(hr = (cmd))) throw DXException(hr); }