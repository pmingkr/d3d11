#pragma once

#include "common.h"

namespace cbs
{
	class SamplerState:public AutoRelease<ID3D11SamplerState>
	{
	public:
		SamplerState();
		SamplerState(D3D11_TEXTURE_ADDRESS_MODE ta, D3D11_FILTER filter);

		using AutoRelease::operator ->;
		using AutoRelease::operator &;
	};
	
}