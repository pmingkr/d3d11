#include "texture.h"
#include "device.h"

#if _MSC_VER >= 1700
#include "WICTextureLoader\WICTextureLoader.h"
#else
#include <D3DX11.h>
#endif

using namespace cbs;

cbs::Texture::Texture()
{
}
cbs::Texture::Texture(const wchar_t * filename)
{
#if _MSC_VER >= 1700
	throwhr(DirectX::CreateWICTextureFromFile(g_device, filename, nullptr, &m_ptr));
#else
	throwhr(D3DX11CreateShaderResourceViewFromFile(g_device, filename, nullptr, nullptr, &m_ptr, nullptr));
#endif
}