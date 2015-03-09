#pragma once

#include "common.h"
#include <map>
#include <string>

namespace cbs
{
	class TextureData:public IUnknown
	{
	public:
		CBS_DX11LIB_EXPORT TextureData();
		CBS_DX11LIB_EXPORT TextureData(TextureData&& move);
		CBS_DX11LIB_EXPORT TextureData& operator =(TextureData&& move);
		CBS_DX11LIB_EXPORT explicit TextureData(const wchar_t * filename);

		CBS_DX11LIB_EXPORT ULONG STDMETHODCALLTYPE AddRef(); //override
		CBS_DX11LIB_EXPORT ULONG STDMETHODCALLTYPE Release(); //override
		CBS_DX11LIB_EXPORT HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void ** ppvObject); // override
		
		CBS_DX11LIB_EXPORT ID3D11ShaderResourceView* getShaderResourceView();
		CBS_DX11LIB_EXPORT static TextureData * load(const wchar_t * filename);

	private:
		TextureData(const TextureData& copy);// = delete
		TextureData& operator =(const TextureData& copy); // = delete

		AutoRelease<ID3D11ShaderResourceView> m_ptr;
		std::wstring m_filename;
		ULONG m_reference;

		static std::map<std::wstring, TextureData*> m_textures;
	};

	class Texture :public AutoRelease<TextureData>
	{
	public:
		CBS_DX11LIB_EXPORT Texture();
		CBS_DX11LIB_EXPORT explicit Texture(const char * filename, int cp = 949); // 인코딩은 한글을 사용
		CBS_DX11LIB_EXPORT explicit Texture(const wchar_t * filename);
	};

}