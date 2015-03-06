#include "include/cbs/d3d11/texture.h"
#include "include/cbs/d3d11/device.h"
#include "encoding.h"

#if _MSC_VER >= 1700
#include "WICTextureLoader\WICTextureLoader.h"
#else
#include <D3DX11.h>
#endif

using namespace cbs;

std::map<std::wstring, TextureData*> cbs::TextureData::m_textures;

cbs::TextureData::TextureData()
{
	m_reference = 0;
}
cbs::TextureData::TextureData(TextureData&& move)
{
	m_filename = std::move(move.m_filename);
	m_ptr = std::move(move.m_ptr);
	m_reference = move.m_reference;
	move.m_reference = 0;
}
TextureData& cbs::TextureData::operator =(TextureData&& move)
{
	this->~TextureData();
	new(this) TextureData(std::move(move));
	return *this;
}
cbs::TextureData::TextureData(const wchar_t * filename)
{
	m_reference = 0;
	
#if _MSC_VER >= 1700
	HRESULT hr = DirectX::CreateWICTextureFromFile(g_device, filename, nullptr, &m_ptr);
#else
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(g_device, filename, nullptr, nullptr, &m_ptr, nullptr);
#endif
	if (SUCCEEDED(hr)) return;

	wchar_t temp[1024];
	swprintf_s(temp, L"%ls 텍스처를 열지 못하였습니다.\r\nHRESULT: 0x%08X\r\n", filename, hr);
	OutputDebugString(temp);
	MessageBox(g_hWnd, temp, nullptr, MB_OK | MB_ICONERROR);
	m_ptr = nullptr;
}

ULONG cbs::TextureData::AddRef() //override
{
	return ++m_reference;
}
ULONG cbs::TextureData::Release() //override
{
	ULONG res = --m_reference;
	if (res == 0)
	{
		m_textures.erase(m_filename);
		delete this;
	}
	return res;
}
HRESULT cbs::TextureData::QueryInterface(REFIID riid, void ** ppvObject) // override
{
	return m_ptr->QueryInterface(riid, ppvObject);
}

ID3D11ShaderResourceView* cbs::TextureData::getShaderResourceView()
{
	return m_ptr;
}
cbs::TextureData * cbs::TextureData::load(const wchar_t * filename)
{
	std::wstring strfilename = filename;
	size_t filelen = strfilename.size();

	auto pair = m_textures.insert(std::pair<std::wstring, TextureData*>(std::move(strfilename), nullptr));
	TextureData * ptr;
	if (pair.second)
	{
		pair.first->second = ptr = new TextureData(filename);
	}
	else
	{
		ptr = pair.first->second;
	}
	ptr->AddRef();
	return ptr;
}

cbs::Texture::Texture()
{
}
cbs::Texture::Texture(const char * filename, int cp)
{
	wchar_t temp[MAX_PATH];
	cnvcpy(temp, filename, cp);	
	m_ptr = TextureData::load(temp);
}
cbs::Texture::Texture(const wchar_t * filename)
{
	m_ptr = TextureData::load(filename);
}