#include "include/cbs/d3d11/texture.h"
#include "include/cbs/d3d11/device.h"
#include "include/cbs/encoding.h"

#if _MSC_VER >= 1700 // VS 2012 이상
#include "WICTextureLoader\WICTextureLoader.h"
#else
#include <D3DX11.h>
#pragma comment(lib, "d3dx11.lib")
#endif

using namespace cbs;
using namespace std;


AutoDelete<map<wstring, TextureData*>> TextureData::m_textures;

TextureData::TextureData()
{
	m_reference = 0;
}
TextureData::TextureData(TextureData&& _move)
{
	m_filename = move(_move.m_filename);
	m_ptr = move(_move.m_ptr);
	m_reference = _move.m_reference;
	_move.m_reference = 0;
}
TextureData& TextureData::operator =(TextureData&& _move)
{
	this->~TextureData();
	new(this) TextureData(move(_move));
	return *this;
}
TextureData::TextureData(const wchar_t * filename)
{
	m_reference = 0;
	m_filename = filename;
	
#if _MSC_VER >= 1700 // VS 2012 이상
	HRESULT hr = DirectX::CreateWICTextureFromFile(g_device, g_context, filename, nullptr, &m_ptr);
#else
	D3DX11_IMAGE_LOAD_INFO ili;
	ili.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	ili.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(g_device, filename, nullptr, nullptr, &m_ptr, nullptr);
#endif
	if (SUCCEEDED(hr)) return;

	wchar_t temp[1024];
	swprintf_s(temp, L"%ls 텍스처를 열지 못하였습니다.\r\nHRESULT: 0x%08X\r\n", filename, hr);
	OutputDebugString(temp);
	MessageBox(nullptr, temp, nullptr, MB_OK | MB_ICONERROR);
	m_ptr = nullptr;
}

ULONG TextureData::AddRef() //override
{
	return ++m_reference;
}
ULONG TextureData::Release() //override
{
	ULONG res = --m_reference;
	if (res == 0)
	{
		if (!m_filename.empty())
		{
			assert(m_textures != nullptr);
			m_textures->erase(m_filename);
		}
		delete this;
	}
	return res;
}
HRESULT TextureData::QueryInterface(REFIID riid, void ** ppvObject) // override
{
	return m_ptr->QueryInterface(riid, ppvObject);
}

TextureData * TextureData::load(const wchar_t * filename)
{
	TextureData * ptr;
	if(filename[0] == L'\0')
	{
		ptr = new TextureData();
		ptr->AddRef();
		return ptr;
	}
	wstring strfilename = filename;
	size_t filelen = strfilename.size();

	auto res = _getTextureMap()->insert(pair<wstring, TextureData*>(move(strfilename), nullptr));
	if (res.second)
	{
		res.first->second = ptr = new TextureData(filename);
	}
	else
	{
		ptr = res.first->second;
	}
	ptr->AddRef();
	return ptr;
}
void TextureData::cleanupMemory()
{
	if(m_textures != nullptr)
	{
		auto iter = m_textures->begin();
		auto end = m_textures->end();
		while (iter != end)
		{
			TextureData * td = iter->second;
			td->m_ptr = nullptr;
			td->m_filename.clear();
			iter++;
		}
		m_textures = nullptr;
	}
}
map<std::wstring, TextureData*> * TextureData::_getTextureMap()
{
	m_textures = new map<wstring, TextureData*>();
	return m_textures;
}
