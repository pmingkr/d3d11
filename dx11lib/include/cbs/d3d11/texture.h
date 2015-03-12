#pragma once

#include "common.h"
#include <map>
#include <string>
#include "../encoding.h"

namespace cbs
{
	class TextureData:public IUnknown
	{
	private:
		TextureData(const TextureData& copy);// = delete
		TextureData& operator =(const TextureData& copy); // = delete

		TextureData();
		TextureData(TextureData&& _move);
		TextureData& operator =(TextureData&& _move);
		explicit TextureData(const wchar_t * filename);

		AutoRelease<ID3D11ShaderResourceView> m_ptr;
		std::wstring m_filename;
		ULONG m_reference;

		static std::map<std::wstring, TextureData*> * _getTextureMap();
		static AutoDelete<std::map<std::wstring, TextureData*>> m_textures;

	public:
		CBS_DX11LIB_EXPORT ULONG STDMETHODCALLTYPE AddRef(); //override
		CBS_DX11LIB_EXPORT ULONG STDMETHODCALLTYPE Release(); //override
		CBS_DX11LIB_EXPORT HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void ** ppvObject); // override
		
		CBS_DX11LIB_EXPORT static TextureData * load(const wchar_t * filename);
		CBS_DX11LIB_EXPORT static void cleanupMemory();

		inline ID3D11ShaderResourceView* getShaderResourceView()
		{
			return m_ptr;
		}
	};

	class Texture :public AutoRelease<TextureData>
	{
	public:
		inline Texture()
		{
		}
		inline explicit Texture(const char * filename, int cp = 949) // 인코딩은 한글을 사
		{
			wchar_t temp[MAX_PATH];
			cnvcpy(temp, filename, cp);
			m_ptr = TextureData::load(temp);
		}
		inline explicit Texture(const wchar_t * filename)
		{
			m_ptr = TextureData::load(filename);
		}
	};

}