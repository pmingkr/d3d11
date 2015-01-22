#include "dx11.h"
#include <fstream>
#include <d3dcompiler.h>

#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3d11.lib")

using namespace std;

template <typename T> auto LoadShader(const wchar_t * strFileName, const char * version, T lambda) -> decltype(lambda((void*)0, (size_t)0))
{
	AutoRelease<ID3D10Blob> blob;
	AutoRelease<ID3D10Blob> bloberr;
	HRESULT hr = D3DX11CompileFromFile(strFileName, nullptr, nullptr, "main", version, 0, 0, nullptr, &blob, &bloberr, nullptr);
	
	if(bloberr != nullptr)
	{
		char *errors = (char*)bloberr->GetBufferPointer();
		OutputDebugStringA(errors);
		MessageBoxA(nullptr, errors, nullptr, MB_OK | MB_ICONERROR);
	}
	if(FAILED(hr)) throw DXException(hr);

	return lambda(blob->GetBufferPointer(), blob->GetBufferSize());
}

D3D11Device::D3D11Device(int width, int height)
	:Window(width, height)
{
	RECT rc;
	GetClientRect(m_hWnd, &rc);

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
	sd.OutputWindow = m_hWnd;
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
				&sd, &m_chain, &m_device, &featureLevel, &m_context);
			if (SUCCEEDED(hr)) goto __succeeded;
		}
		throw DXException(hr);
	}
__succeeded:

	{
		AutoRelease<ID3D11Texture2D> pBackBuffer;
		throwhr(m_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer));
		throwhr(m_device->CreateRenderTargetView(pBackBuffer, nullptr, &m_rtv));
		m_context->OMSetRenderTargets(1, &m_rtv, nullptr);
	}

	D3D11_VIEWPORT vp;
	vp.Width = (float)rc.right;
	vp.Height = (float)rc.bottom;
	vp.MaxDepth = 1;
	vp.MinDepth = 0;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_context->RSSetViewports(1, &vp);
}
D3D11Device::~D3D11Device()
{
#ifdef _DEBUG
	m_rtv = nullptr;
	m_chain = nullptr;

	m_context->ClearState();
	m_context = nullptr;

	AutoRelease<ID3D11Debug> d3ddebug;
	m_device->QueryInterface(__uuidof(ID3D11Debug), (void**)&d3ddebug);
	m_device = nullptr;

	//d3ddebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);

#endif
}

AutoRelease<ID3D11VertexShader> D3D11Device::LoadVertexShader(LPCTSTR strFileName, ID3D11InputLayout **ppLayout,
	const D3D11_INPUT_ELEMENT_DESC * desc, size_t count)
{
	auto lambda = [&](void * data, size_t size) ->AutoRelease<ID3D11VertexShader>
	{
		AutoRelease<ID3D11VertexShader> pVS;
		throwhr(m_device->CreateVertexShader(data, size, nullptr, &pVS));
		throwhr(m_device->CreateInputLayout(desc, count, data, size, ppLayout));
		return pVS;
	};

	return LoadShader(strFileName, "vs_4_0", lambda);
}
AutoRelease<ID3D11PixelShader> D3D11Device::LoadPixelShader(LPCTSTR strFileName)
{
	auto lambda = [this](void * data, size_t size) ->AutoRelease<ID3D11PixelShader>
	{
		AutoRelease<ID3D11PixelShader> pPS;
		throwhr(m_device->CreatePixelShader(data, size, nullptr, &pPS));
		return pPS;
	};
	return LoadShader(strFileName, "ps_4_0", lambda);
}

DXException::DXException(HRESULT hr)
{
	m_hr = hr;
}
DXException::operator HRESULT()
{
	return m_hr;
}

Buffer::Buffer()
{
}
Buffer::Buffer(ID3D11Device * device, UINT bindflags, size_t size)
{
	allocate(device, bindflags, size);
}
Buffer::Buffer(ID3D11Device * device, UINT bindflags, size_t size, const void * initdata)
{
	allocate(device, bindflags, size, initdata);
}

void Buffer::allocate(ID3D11Device * device, UINT bindflags, size_t size)
{
	D3D11_BUFFER_DESC bd;
	memset(&bd, 0, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;
	bd.ByteWidth = size;
	bd.BindFlags = bindflags;
	throwhr(device->CreateBuffer(&bd, nullptr, &m_buffer));
}
void Buffer::allocate(ID3D11Device * device, UINT bindflags, size_t size, const void * initdata)
{
	D3D11_SUBRESOURCE_DATA sd;
	memset(&sd, 0, sizeof(sd));
	sd.pSysMem = initdata;

	D3D11_BUFFER_DESC bd;
	memset(&bd, 0, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;
	bd.ByteWidth = size;
	bd.BindFlags = bindflags;

	throwhr(device->CreateBuffer(&bd, &sd, &m_buffer));
}

ID3D11Buffer** Buffer::operator &()
{
	return &m_buffer;
}
Buffer::operator ID3D11Buffer *()
{
	return m_buffer;
}
ID3D11Buffer * Buffer::operator ->()
{
	return m_buffer;
}