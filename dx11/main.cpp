#include "main.h"

#include "ps_basic.h"
#include "ps_notexture.h"
#include "vs_basic.h"
#include "vs_skinned.h"

using namespace cbs;

// vs_common.hlsli ��� ����ü
struct CB_VSCommon
{
	XMMATRIX mVP;
};

// vshader.hlsl ��� ����ü
struct CB_VSBasic
{
	XMMATRIX mWorlds;
};

// vs_skinned.hlsl ��� ����ü
struct CB_VSSkinned
{
	float mWorlds[3][4][80];
};

// pshader.hlsl ��� ����ü
struct CB_PSBasic
{
	XMFLOAT4 vColor;
};

// ���� ����ü
struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 texcoord;
	XMBYTE4 blendidx;
	XMFLOAT4 blendwgt;
};

class �ؾ
{
private:
	const char * �ؾ��_�̸�;
	int �ؾ_ũ��;
	int ������_��;

public:
	void ������()
	{
		�ؾ_ũ�� = 0;
		������_�� = 0;
	}
	void �Ա�(int �Դ¾�)
	{
		�ؾ_ũ�� -= �Դ¾�;
		������_�� -= �Դ¾�/2;
	}

};

class Model3D
{
private:

public:
	// �����ϱ�
	void create(const char * name)
	{
	}

	// ȭ�鿡_���
	void render(float x,float y)
	{
		
	}
};

void main()
{
	�ؾ �ؾ�A;
	�ؾ�A.������();
}

Main::Main() :D3D11Device(800, 600)
{		
	// �簢�� ���� ���� ����
	static const float vertices[] =
	{
		-1, 1, 0,	0, 0,
		1, 1, 0,	1, 0,
		-1, -1, 0,	0, 1,
		1, -1, 0,	1, 1
	};
	m_vbQuad = Buffer(D3D11_BIND_VERTEX_BUFFER, vertices);

	// ���̴� ��� ����
	m_cb_vsCommon = Buffer(D3D11_BIND_CONSTANT_BUFFER, sizeof(CB_VSCommon));
	m_cb_vsBasic = Buffer(D3D11_BIND_CONSTANT_BUFFER, sizeof(CB_VSBasic));
	m_cb_psBasic = Buffer(D3D11_BIND_CONSTANT_BUFFER, sizeof(CB_PSBasic));
	m_cb_vsSkinned = Buffer(D3D11_BIND_CONSTANT_BUFFER, sizeof(CB_VSSkinned));

	// basic ���� ���̴� & ���� ���̾ƿ�
	static const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, normal), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex, texcoord), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	m_vs_basic = VertexShader(s_vs_basic, layout); // ���� ���̴�

	// skinned ���� ���̴� & ���� ���̾ƿ�
	static const D3D11_INPUT_ELEMENT_DESC layout_skinned[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, normal), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex, texcoord), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, offsetof(Vertex, blendidx), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(Vertex, blendwgt), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	m_vs_skinned = VertexShader(s_vs_skinned, layout_skinned);

	m_ps_basic = PixelShader(s_ps_basic); // �ȼ� ���̴�
	m_ps_noTexture = PixelShader(s_ps_notexture); // �ؽ�ó ���� �ȼ� ���̴�
	m_sam = SamplerState(D3D11_TEXTURE_ADDRESS_WRAP, D3D11_FILTER_MIN_MAG_MIP_LINEAR); // �Ӹ�
	m_depth = DepthStencilState(D3D11_COMPARISON_LESS, D3D11_DEPTH_WRITE_MASK_ALL); // ���� ���ٽ� ������Ʈ
	//m_model = Model("models/Ogre/TheThing/Mesh.mesh.xml");

	//m_dwarf = Model("models-nonbsd/Ogre/OgreSDK/fish.mesh.xml");
	//m_dwarf = Model("models-nonbsd/MD5/Bob.md5mesh");
	m_dwarf = Model("models-nonbsd/X/dwarf.x");
	m_model = Model("res/tiny_4anim.x");
	m_model.setAnimationTPS(4800.0);

	m_modelTime = 0;
	m_dwarfTime = 0;
}
Main::~Main()
{
}
void Main::loop()
{
	float delta = (float)m_delta.measureDelta();

	clear(); // ȭ�� �ʱ�ȭ

	// �ؽ�ó ���÷� ����
	g_context->PSSetSamplers(0, 1, &m_sam);

	// ���� ���� ��� ����
	g_context->OMSetDepthStencilState(m_depth, 0);

	// ���̴� ��� ����
	g_context->VSSetConstantBuffers(0, 1, &m_cb_vsCommon);
	g_context->PSSetConstantBuffers(0, 1, &m_cb_psBasic);

	// ��/�������� ��� ����
	{
		XMMATRIX mVP = XMMatrixLookAtLH(vector(-1000.f, 0, 0), vector(0, 0, 0), vector(0, 0, -1));
		mVP *= XMMatrixPerspectiveFovLH(XM_PI/3, 800/600.f, 50.f, 10000.f);
		
		CB_VSCommon sc;
		sc.mVP = XMMatrixTranspose(mVP);
		g_context->UpdateSubresource(m_cb_vsCommon, 0, nullptr, &sc, 0, 0);
	}
	aiMatrix4x4 mTmp;
	aiMatrix4x4 mRes;
	mRes = aiMatrix4x4::Translation(aiVector3D(200.f, 0.f, 200.f), mTmp);
	mRes *= aiMatrix4x4::RotationZ(GetTickCount() / 800.f, mTmp);
	drawModel(delta, &m_model, &m_modelTime, mRes); // tiny rendering

	// 180 = XM_PI
	// 90 = XM_PI/2
	// -90 = -XM_PI/2

	mRes = aiMatrix4x4::Translation(aiVector3D(500.f, 0.f, 200.f), mTmp);
	mRes *= aiMatrix4x4::RotationZ(GetTickCount() / 800.f, mTmp);
	mRes *= aiMatrix4x4::Scaling(aiVector3D(10.f, 10.f, 10.f), mTmp);
	drawModel(delta, &m_dwarf, &m_dwarfTime, mRes); // dwarf rendering

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

	// �ȼ� ���̴� ����
	switch (mtl.texCount)
	{
	case 0:
		g_context->PSSetShader(m_ps_noTexture, nullptr, 0);
		break;
	default:
		g_context->PSSetShader(m_ps_basic, nullptr, 0);
		break;
	}

	// �ȼ� ���̴� ��� ����
	CB_PSBasic sc;
	sc.vColor = XMFLOAT4(0.5f, 0.5f, 0.5f, 1);
	g_context->UpdateSubresource(m_cb_psBasic, 0, nullptr, &sc, 0, 0);
}
void Main::setWorld(const aiMatrix4x4 & matrix)
{
	// ���ؽ� ���̴� ����
	g_context->VSSetShader(m_vs_basic, nullptr, 0);
	g_context->VSSetConstantBuffers(1, 1, &m_cb_vsBasic);
	g_context->IASetInputLayout(m_vs_basic.getInputLayer());

	CB_VSBasic sc;
	memcpy(&sc.mWorlds, &matrix, sizeof(XMMATRIX));
	g_context->UpdateSubresource(m_cb_vsBasic, 0, nullptr, &sc, 0, 0);
}
void Main::setBoneWorlds(const Matrix4x3 * matrix, size_t m4x3count)
{
	// ���ؽ� ���̴� ����
	g_context->VSSetShader(m_vs_skinned, nullptr, 0);
	g_context->VSSetConstantBuffers(1, 1, &m_cb_vsSkinned);
	g_context->IASetInputLayout(m_vs_skinned.getInputLayer());

	CB_VSSkinned sc;
	memcpy(&sc.mWorlds, matrix, m4x3count * (sizeof(float)*4*3));
	g_context->UpdateSubresource(m_cb_vsSkinned, 0, nullptr, &sc, 0, 0);
}
void Main::drawModel(float delta, cbs::Model * model, double * time, aiMatrix4x4 matrix)
{
	// Ÿ�̴� ������
	{
		//Object a;
		//Object * b;
		// a.function();
		// b->function();

		// �ִϸ��̼� ���
		Model::Pose pose;
		Model::AnimationStatus status;
		status.animation = model->getAnimation(0); // �ִϸ��̼� ��������
		status.time = *time + delta; // �ִϸ��̼� �ð� �̵�
		pose.set(&status); // ���� ��������
		*time = status.time; // �ִϸ��̼� �ð� ����

		// ���� ��� ����
		pose.transform(matrix);

		// ������
		render(*model, pose);
	}	
}