#pragma once

#include "common.h"
#include <map>
#include <string>

namespace cbs
{
	class TextureData:public IUnknown
	{
	public:
		TextureData();
		TextureData(TextureData&& move);
		TextureData& operator =(TextureData&& move);
		explicit TextureData(const wchar_t * filename);

		ULONG STDMETHODCALLTYPE AddRef(); //override
		ULONG STDMETHODCALLTYPE Release(); //override
		HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void ** ppvObject); // override
		
		ID3D11ShaderResourceView* getShaderResourceView();
		static TextureData * load(const wchar_t * filename);

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
		Texture();
		explicit Texture(const char * filename, int cp = 949); // 인코딩은 한글을 사용
		explicit Texture(const wchar_t * filename);
	};

}