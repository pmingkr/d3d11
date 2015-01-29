#include "dx11.h"
#include <fstream>


#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3d11.lib")

using namespace std;

using namespace cbs;

AutoRelease<ID3D11Device>			cbs::g_device;
AutoRelease<ID3D11DeviceContext>	cbs::g_context;
AutoRelease<IDXGISwapChain>			cbs::g_chain;
AutoRelease<ID3D11RenderTargetView>	cbs::g_rtv;

D3D11Device::D3D11Device(int width, int height)
	:Window(width, height)
{
	if (g_context != nullptr) throw DuplicationException();

	RECT rc;
	GetClientRect(g_hWnd, &rc);

	UINT uFlags = 0;
#ifdef _DEBUG
	uFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};
	D3D_FEATURE_LEVEL featureLevel;

	DXGI_SWAP_CHAIN_DESC sd;
	memset(&sd, 0, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = rc.right;
	sd.BufferDesc.Height = rc.bottom;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};

	{
		HRESULT hr;
		for (int i = 0; i<3; i++)
		{
			hr = D3D11CreateDeviceAndSwapChain(nullptr, driverTypes[i], nullptr, uFlags, featureLevels, 6, D3D11_SDK_VERSION, 
				&sd, &g_chain, &g_device, &featureLevel, &g_context);
			if (SUCCEEDED(hr)) goto __succeeded;
		}
		throw DXException(hr);
	}
__succeeded:

	{
		AutoRelease<ID3D11Texture2D> pBackBuffer;
		throwhr(g_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer));
		throwhr(g_device->CreateRenderTargetView(pBackBuffer, nullptr, &g_rtv));
		g_context->OMSetRenderTargets(1, &g_rtv, nullptr);
	}

	D3D11_VIEWPORT vp;
	vp.Width = (float)rc.right;
	vp.Height = (float)rc.bottom;
	vp.MaxDepth = 1;
	vp.MinDepth = 0;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_context->RSSetViewports(1, &vp);
}
D3D11Device::~D3D11Device()
{
	g_rtv = nullptr;
	g_chain = nullptr;
	if (g_context != nullptr)
	{
		g_context->ClearState();
		g_context = nullptr;
	}

#ifdef _DEBUG

	AutoRelease<ID3D11Debug> d3ddebug;
	g_device->QueryInterface(__uuidof(ID3D11Debug), (void**)&d3ddebug);
	d3ddebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);

#endif

	g_device = nullptr;
}

DXException::DXException(HRESULT hr)
{
	m_hr = hr;
}
DXException::operator HRESULT()
{
	return m_hr;
}
