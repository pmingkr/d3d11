#pragma once

#include "common.h"

namespace cbs
{
	class Texture:public AutoRelease<ID3D11ShaderResourceView>
	{
	public:
		Texture();
		explicit Texture(const wchar_t * filename);

		using AutoRelease::operator->;
		using AutoRelease::operator&;

	};

}