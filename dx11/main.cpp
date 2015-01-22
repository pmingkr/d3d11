#include "main.h"
#include <sstream>
#include <iomanip>

const int WIDTH = 800;
const int HEIGHT = 600;

using namespace std;

struct ShaderConst
{
	XMFLOAT4 vColor;
};

Main::Main()
	:D3D11Device(WIDTH, HEIGHT)
{
	static const float vertices[] =
	{
		0, 1, 0 ,
		1, -1, 0,
		-1, -1, 0
	};

	static const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	
	m_vb = Buffer(m_device, D3D11_BIND_VERTEX_BUFFER, vertices);
	m_constbuffer = Buffer(m_device, D3D11_BIND_CONSTANT_BUFFER, sizeof(ShaderConst));

	m_vs = LoadVertexShader(L"shaders/vshader.hlsl", &m_layout, layout);
	m_ps = LoadPixelShader(L"shaders/pshader.hlsl");
}
Main::~Main()
{
}
void Main::loop()
{
	float colors[4] = { 0,0,0,1 };
	m_context->ClearRenderTargetView(m_rtv, colors);

	m_context->VSSetShader(m_vs, nullptr, 0);
	m_context->PSSetShader(m_ps, nullptr, 0);

	UINT stride = sizeof(float) * 3;
	UINT offset = 0;
	m_context->IASetVertexBuffers(0, 1, &m_vb, &stride, &offset);
	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_context->IASetInputLayout(m_layout);

	ShaderConst sc;
	sc.vColor = XMFLOAT4(1, 0, 0, 1);
	m_context->UpdateSubresource(m_constbuffer, 0, nullptr, &sc, 0, 0);
	m_context->VSSetConstantBuffers(0, 1, &m_constbuffer);
	m_context->PSSetConstantBuffers(0, 1, &m_constbuffer);

	m_context->Draw(3, 0);
	
	m_chain->Present(1, 0);
}

int forSEH()
{
	try
	{
		Main main;
		int res = main.messageLoop();
		_CrtDumpMemoryLeaks();
		return res;
	}
	catch (DXException & ex)
	{
		wstringstream ss;
		ss << L"오류가 났소. \r\n오류코드: 0x" << hex << (HRESULT)ex;
		MessageBox(nullptr, ss.str().c_str(), nullptr, MB_OK | MB_ICONERROR);
		return (int)(HRESULT)ex;
	}
}
int WINAPI wWinMain(HINSTANCE hInstance,HINSTANCE,LPWSTR,int)
{
	g_hInst = hInstance;
	__try
	{
		return forSEH();
	}
	__except(GetExceptionCode() != EXCEPTION_BREAKPOINT ? EXCEPTION_ACCESS_VIOLATION : EXCEPTION_EXECUTE_HANDLER)
	{
		MessageBox(nullptr, L"죽었소 ㅠㅠ", nullptr, MB_OK | MB_ICONERROR);
		return -1;
	}
}
