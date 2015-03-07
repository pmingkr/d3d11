#include "include/cbs/d3d11/state.h"
#include "include/cbs/d3d11/device.h"

namespace
{
	template <typename T> int makeIndex(T value)
	{
		typedef cbs::StateConst<T> C;
		int res = value - C::offset;
		assert(res < C::count);
		return res;
	}
	template <> int makeIndex<D3D11_FILTER>(D3D11_FILTER value)
	{
		int res;
		switch (value)
		{
		case D3D11_FILTER_MIN_MAG_MIP_POINT: res = 0; break;
		case D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR: res = 1; break;
		case D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT: res = 2; break;
		case D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR: res = 3; break;
		case D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT: res = 4; break;
		case D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR: res = 5; break;
		case D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT: res = 6; break;
		case D3D11_FILTER_MIN_MAG_MIP_LINEAR: res = 7; break;
		case D3D11_FILTER_ANISOTROPIC: res = 8; break;
			//case D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT: res = 10;
			//case D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR: res = 11;
			//case D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT: res = 12;
			//case D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR: res = 13;
			//case D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT: res = 14;
			//case D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR: res = 15;
			//case D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT: res = 16;
			//case D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR: res = 17;
			//case D3D11_FILTER_COMPARISON_ANISOTROPIC: res = 18;
		default: assert(!"unnkown value"); res = 0; break;
		}
		assert(res < cbs::StateConst<D3D11_FILTER>::count);
		return res;
	}
	template <> int makeIndex<bool>(bool value)
	{
		return value ? 1 : 0;
	}

	class IndexMaker
	{
	private:
		int index;
		int maximum;

	public:
		IndexMaker()
		{
			index = 0;
			maximum = 1;
		}
		template <typename T>
		IndexMaker(T value)
		{
			index = makeIndex(value);
			maximum = cbs::StateConst<T>::count;
		}
		template <typename T>
		IndexMaker& operator |=(T value)
		{
			typedef cbs::StateConst<T> C;
			index += makeIndex(value) * maximum;
			maximum *= C::count;
			return *this;
		}
		template <typename T>
		const IndexMaker operator |(T value) const
		{
			IndexMaker out = *this;
			out |= value;
			return out;
		}
		template <typename T>
		friend const IndexMaker operator |(T value, const IndexMaker im)
		{
			im |= value;
			return im;
		}

		operator int() const
		{
			return index;
		}
	};

}

cbs::StateContainer cbs::StateContainer::m_container;

cbs::SamplerState::SamplerState()
{
	m_ptr = nullptr;
}
cbs::SamplerState::SamplerState(D3D11_TEXTURE_ADDRESS_MODE ta, D3D11_FILTER filter)
{
	int index = (IndexMaker)ta | filter;
	assert(index < _countof(StateContainer::m_container.samplers));
	ID3D11SamplerState * & s = StateContainer::m_container.samplers[index];
	if (s == nullptr)
	{
		D3D11_SAMPLER_DESC sd;
		memset(&sd, 0, sizeof(sd));
		sd.AddressU = ta;
		sd.AddressV = ta;
		sd.AddressW = ta;
		sd.Filter = filter;

		if(filter == D3D11_FILTER_ANISOTROPIC)
		{
			sd.MaxAnisotropy = 4;
		}

		sd.MinLOD = 0;
		sd.MaxLOD = D3D11_FLOAT32_MAX;

		throwhr(g_device->CreateSamplerState(&sd, &s));
	}
	m_ptr = s;
}

cbs::RasterizerState::RasterizerState()
{
	m_ptr = nullptr;
}
cbs::RasterizerState::RasterizerState(D3D11_CULL_MODE cull, D3D11_FILL_MODE fill, bool depthClipping)
{
	int index = (IndexMaker)cull | fill;
	assert(index < _countof(StateContainer::m_container.rasterizer));
	ID3D11RasterizerState * & r = StateContainer::m_container.rasterizer[index];
	if (r == nullptr)
	{
		D3D11_RASTERIZER_DESC rd;
		memset(&rd, 0, sizeof(rd));
		rd.CullMode = cull;
		rd.FillMode = fill;
		rd.DepthClipEnable = depthClipping;
		throwhr(g_device->CreateRasterizerState(&rd, &r));
	}
	m_ptr = r;
}

cbs::DepthStencilState::DepthStencilState()
{
}
cbs::DepthStencilState::DepthStencilState(D3D11_COMPARISON_FUNC cmp, D3D11_DEPTH_WRITE_MASK mask)
{
	int index = (IndexMaker)cmp | mask;
	assert(index < _countof(StateContainer::m_container.depthStencil));
	ID3D11DepthStencilState * & d = StateContainer::m_container.depthStencil[index];
	if (d == nullptr)
	{
		D3D11_DEPTH_STENCIL_DESC dsd;
		memset(&dsd, 0, sizeof(dsd));
		dsd.DepthEnable = true;
		dsd.DepthFunc = cmp;
		dsd.DepthWriteMask = mask;
		throwhr(g_device->CreateDepthStencilState(&dsd, &d));
	}
	m_ptr = d;
}

void cbs::StateContainer::clear()
{
	SafeReleaseArray(m_container.rasterizer);
	SafeReleaseArray(m_container.samplers);
	SafeReleaseArray(m_container.depthStencil);
}
