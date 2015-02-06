#include "samplerstate.h"
#include "device.h"

cbs::SamplerState::SamplerState()
{
}
cbs::SamplerState::SamplerState(D3D11_TEXTURE_ADDRESS_MODE ta, D3D11_FILTER filter)
{
	D3D11_SAMPLER_DESC sd;
	memset(&sd,0, sizeof(sd));
	sd.AddressU = ta;
	sd.AddressV = ta;
	sd.AddressW = ta;
	sd.Filter = filter;
	throwhr(g_device->CreateSamplerState(&sd, &m_ptr));
}