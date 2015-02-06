#include "main.h"

#include "pshader.h"
#include "vshader.h"

using namespace cbs;

// ���̴� ��� ����ü
struct ShaderConst
{
	XMFLOAT4 vColor;
};

// ���� ����ü
struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT2 tex;
};

Main::Main() :D3D11Device(800, 600)
{		
	// ���ý� ���� ����
	static const float vertices[] =
	{
		-1, 1, 0,	0, 0,
		1, 1, 0,	1, 0,
		-1, -1, 0,	0, 1,
		1, -1, 0,	1, 1
	};
	m_vbQuad = Buffer(D3D11_BIND_VERTEX_BUFFER, vertices);

	// ���̴� ��� ����
	m_constbuffer = Buffer(D3D11_BIND_CONSTANT_BUFFER, sizeof(ShaderConst));

	// ���� ���̴� �ε� �� ���ؽ� ��ǲ ���̾� ����
	static const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex, tex), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	m_vs = VertexShader(s_vshader, layout);

	// �ȼ� ���̴� �ε�
	m_ps = PixelShader(s_pshader);

	// �ؽ�ó �ε�
	m_tex = Texture(L"test.BMP");

	// ���÷� ����
	m_sam = SamplerState(D3D11_TEXTURE_ADDRESS_WRAP, D3D11_FILTER_MIN_MAG_MIP_LINEAR);
}
Main::~Main()
{
}
void Main::loop()
{
	// �� ���� �����
	float colors[4] = { 0,0,0,1 };
	g_context->ClearRenderTargetView(g_rtv, colors);

	// ���̴� ����
	g_context->VSSetShader(m_vs, nullptr, 0);
	g_context->IASetInputLayout(m_vs.getInputLayer());
	g_context->PSSetShader(m_ps, nullptr, 0);

	// ���ؽ� ���� ����
	UINT stride = sizeof(float) * 5;
	UINT offset = 0;
	g_context->IASetVertexBuffers(0, 1, &m_vbQuad, &stride, &offset);

	// ������Ƽ�� �������� ����
	g_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	
	// �ؽ�ó ����
	g_context->PSSetShaderResources(0, 1, &m_tex);

	// �ؽ�ó ���÷� ����
	g_context->PSSetSamplers(0, 1, &m_sam);

	// ���̴� ��� ������Ʈ
	ShaderConst sc;
	sc.vColor = XMFLOAT4(0.5f, 1, 1, 1);
	g_context->UpdateSubresource(m_constbuffer, 0, nullptr, &sc, 0, 0);
	g_context->VSSetConstantBuffers(0, 1, &m_constbuffer);
	g_context->PSSetConstantBuffers(0, 1, &m_constbuffer);

	// ������
	g_context->Draw(4, 0);
	
	// ������Ʈ
	g_chain->Present(1, 0);
}
