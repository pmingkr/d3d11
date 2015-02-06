#include "texture.h"
#include "device.h"

#pragma warning(disable:4005)
#pragma warning(disable:4458)
#include <d3dtypes.h>
#include <dxgi.h>
#include <d2d1.h>
#include "WICTextureLoader\WICTextureLoader.h"

using namespace cbs;

cbs::Texture::Texture()
{
}
cbs::Texture::Texture(const wchar_t * filename)
{
	throwhr(DirectX::CreateWICTextureFromFile(g_device, filename, nullptr, &m_ptr));
}