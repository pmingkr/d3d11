#include "main.h"

#include "pshader.h"
#include "vshader.h"

using namespace cbs;

// ���̴� ��� ����ü
struct ShaderConst
{
	XMFLOAT4 vColor;
	XMMATRIX mWVP;
};

// ���� ����ü
struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT3 norm;
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
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, norm), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex, tex), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	m_vs = VertexShader(s_vshader, layout); // ���� ���̴�
	m_ps = PixelShader(s_pshader); // �ȼ� ���̴�
	m_sam = SamplerState(D3D11_TEXTURE_ADDRESS_WRAP, D3D11_FILTER_MIN_MAG_MIP_LINEAR); // �Ӹ�
	m_depth = DepthStencilState(D3D11_COMPARISON_LESS, D3D11_DEPTH_WRITE_MASK_ALL); // ���� ���ٽ� ������Ʈ
	//m_model = Model("models/Ogre/TheThing/Mesh.mesh.xml");
	m_model = Model("res/tiny_4anim.x");
	//m_model = Model("models-nonbsd/X/dwarf.x");

	m_startTick = GetTickCount();
}
Main::~Main()
{
}
void Main::loop()
{
	// �� ���� �����
	float colors[4] = { 0,0,0,1 };
	g_context->ClearRenderTargetView(g_rtv, colors);
	g_context->ClearDepthStencilView(g_dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	// ���̴� ����
	g_context->VSSetShader(m_vs, nullptr, 0);
	g_context->IASetInputLayout(m_vs.getInputLayer());
	g_context->PSSetShader(m_ps, nullptr, 0);

	// �ؽ�ó ���÷� ����
	g_context->PSSetSamplers(0, 1, &m_sam);

	// ���� ���� ��� ����
	g_context->OMSetDepthStencilState(m_depth, 0);

	// ���̴� ��� ����
	g_context->VSSetConstantBuffers(0, 1, &m_constbuffer);
	g_context->PSSetConstantBuffers(0, 1, &m_constbuffer);

	// ������
	aiMatrix4x4 mTmp, mRes;
	//float scale = 0.03f;
	float scale = 0.003f;
	//float scale = 100.f / (GetTickCount() - m_startTick + 100);

	mRes = aiMatrix4x4::Translation(aiVector3D(0, -0.5f, 0.5f), mTmp);
	mRes *= aiMatrix4x4::Scaling(aiVector3D(scale, scale, scale*0.01f), mTmp);
	mRes *= aiMatrix4x4::RotationX(XM_PI * -0.3f, mTmp);
	mRes *= aiMatrix4x4::RotationY(GetTickCount() / 800.f, mTmp);
	//render(m_model, mRes);
	render(m_model,  mRes, 1, (GetTickCount() - m_startTick) / 1000.0);
	
	// ������Ʈ
	g_chain->Present(1, 0);
}
void Main::setMaterial(const cbs::Material & mtl)
{
	ID3D11ShaderResourceView * srv[Material::MAX_TEXTURE];
	for (size_t i = 0; i < mtl.texCount; i++)
	{
		srv[i] = mtl.textures[i]->getShaderResourceView();
	}
	g_context->PSSetShaderResources(0, mtl.texCount, srv);
}
void Main::setWorld(const aiMatrix4x4 & matrix)
{
	ShaderConst sc;
	sc.vColor = XMFLOAT4(0.5f, 0.5f, 0.5f, 1);
	memcpy(&sc.mWVP, &matrix, sizeof(XMMATRIX));
	g_context->UpdateSubresource(m_constbuffer, 0, nullptr, &sc, 0, 0);
}
