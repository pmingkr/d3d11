#include "main.h"

#include "pshader.h"
#include "vshader.h"

using namespace cbs;

// 셰이더 상수 구조체
struct ShaderConst
{
	XMFLOAT4 vColor;
};

// 정점 구조체
struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT2 tex;
};

Main::Main() :D3D11Device(800, 600)
{		
	// 버택스 버퍼 생성
	static const float vertices[] =
	{
		-1, 1, 0,	0, 0,
		1, 1, 0,	1, 0,
		-1, -1, 0,	0, 1,
		1, -1, 0,	1, 1
	};
	m_vbQuad = Buffer(D3D11_BIND_VERTEX_BUFFER, vertices);

	// 셰이더 상수 생성
	m_constbuffer = Buffer(D3D11_BIND_CONSTANT_BUFFER, sizeof(ShaderConst));

	// 정점 셰이더 로드 및 버텍스 인풋 레이어 생성
	static const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex, tex), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	m_vs = VertexShader(s_vshader, layout);

	// 픽셀 셰이더 로드
	m_ps = PixelShader(s_pshader);

	// 텍스처 로드
	m_tex = Texture(L"test.BMP");

	// 샘플러 생성
	m_sam = SamplerState(D3D11_TEXTURE_ADDRESS_WRAP, D3D11_FILTER_MIN_MAG_MIP_LINEAR);
}
Main::~Main()
{
}
void Main::loop()
{
	// 색 버퍼 지우기
	float colors[4] = { 0,0,0,1 };
	g_context->ClearRenderTargetView(g_rtv, colors);

	// 셰이더 설정
	g_context->VSSetShader(m_vs, nullptr, 0);
	g_context->IASetInputLayout(m_vs.getInputLayer());
	g_context->PSSetShader(m_ps, nullptr, 0);

	// 버텍스 버퍼 설정
	UINT stride = sizeof(float) * 5;
	UINT offset = 0;
	g_context->IASetVertexBuffers(0, 1, &m_vbQuad, &stride, &offset);

	// 프리미티브 토폴로지 설정
	g_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	
	// 텍스처 설정
	g_context->PSSetShaderResources(0, 1, &m_tex);

	// 텍스처 샘플러 설정
	g_context->PSSetSamplers(0, 1, &m_sam);

	// 셰이더 상수 업데이트
	ShaderConst sc;
	sc.vColor = XMFLOAT4(0.5f, 1, 1, 1);
	g_context->UpdateSubresource(m_constbuffer, 0, nullptr, &sc, 0, 0);
	g_context->VSSetConstantBuffers(0, 1, &m_constbuffer);
	g_context->PSSetConstantBuffers(0, 1, &m_constbuffer);

	// 렌더링
	g_context->Draw(4, 0);
	
	// 프레젠트
	g_chain->Present(1, 0);
}
