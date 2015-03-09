#include "include/cbs/d3d11/device.h"
#include "include/cbs/d3d11/state.h"

using namespace cbs;

AutoRelease<ID3D11Device>			cbs::g_device;
AutoRelease<ID3D11DeviceContext>	cbs::g_context;
AutoRelease<IDXGISwapChain>			cbs::g_chain;
AutoRelease<ID3D11RenderTargetView>	cbs::g_rtv;
AutoRelease<ID3D11DepthStencilView>	cbs::g_dsv;

D3D11Device::D3D11Device(int width, int height, int multiSampling)
	:Window(width, height)
{
	if (g_context != nullptr) throw DuplicationException();

	RECT rc;
	GetClientRect(getWindowHandle(), &rc);

	assert(multiSampling >= 0);
	assert(multiSampling <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT);

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
	sd.OutputWindow = getWindowHandle();
	sd.SampleDesc.Count = multiSampling;
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
#ifdef _DEBUG
			hr = D3D11CreateDeviceAndSwapChain(nullptr, driverTypes[i], nullptr, D3D11_CREATE_DEVICE_DEBUG, featureLevels, 6, D3D11_SDK_VERSION,
				&sd, &g_chain, &g_device, &featureLevel, &g_context);
			if (SUCCEEDED(hr)) goto __succeeded;
#endif
			hr = D3D11CreateDeviceAndSwapChain(nullptr, driverTypes[i], nullptr, 0, featureLevels, 6, D3D11_SDK_VERSION,
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
		
		AutoRelease<ID3D11Texture2D> pDepthStencil;

		D3D11_TEXTURE2D_DESC descDepth;
		memset(&descDepth, 0, sizeof(descDepth));
		descDepth.Width = rc.right;
		descDepth.Height = rc.bottom;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.SampleDesc.Count = multiSampling;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		throwhr(g_device->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

		throwhr(g_device->CreateDepthStencilView(pDepthStencil, nullptr, &g_dsv));

		g_context->OMSetRenderTargets(1, &g_rtv, g_dsv);
	}

	D3D11_VIEWPORT vp;
	vp.Width = (float)rc.right;
	vp.Height = (float)rc.bottom;
	vp.MaxDepth = 1.f;
	vp.MinDepth = 0.f;
	vp.TopLeftX = 0.f;
	vp.TopLeftY = 0.f;
	g_context->RSSetViewports(1, &vp);
}
D3D11Device::~D3D11Device()
{
	StateContainer::clear();

	g_rtv = nullptr;
	g_dsv = nullptr;
	g_chain = nullptr;
	if (g_context != nullptr)
	{
		g_context->ClearState();
		g_context = nullptr;
	}

#ifdef _DEBUG

	//AutoRelease<ID3D11Debug> d3ddebug;
	//g_device->QueryInterface(__uuidof(ID3D11Debug), (void**)&d3ddebug);
	//d3ddebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);

#endif

	g_device = nullptr;
}
void D3D11Device::clear()
{
	// 색 버퍼 지우기
	float colors[4] = { 0,0,0,1 };
	g_context->ClearRenderTargetView(g_rtv, colors);
	g_context->ClearDepthStencilView(g_dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}
