#include "main.h"

#include "ps_basic.h"
#include "ps_notexture.h"
#include "vs_basic.h"
#include "vs_skinned.h"

using namespace cbs;

// vs_common.hlsli 상수 구조체
struct CB_VSCommon
{
	XMMATRIX mVP;
};

// vshader.hlsl 상수 구조체
struct CB_VSBasic
{
	XMMATRIX mWorlds;
};

// vs_skinned.hlsl 상수 구조체
struct CB_VSSkinned
{
	float mWorlds[3][4][80];
};

// pshader.hlsl 상수 구조체
struct CB_PSBasic
{
	XMFLOAT4 vColor;
};

// 정점 구조체
struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 texcoord;
	XMBYTE4 blendidx;
	XMFLOAT4 blendwgt;
};

class 붕어빵
{
private:
	const char * 붕어빵의_이름;
	int 붕어빵_크기;
	int 단팥의_양;

public:
	void 버리기()
	{
		붕어빵_크기 = 0;
		단팥의_양 = 0;
	}
	void 먹기(int 먹는양)
	{
		붕어빵_크기 -= 먹는양;
		단팥의_양 -= 먹는양/2;
	}

};

class Model3D
{
private:

public:
	// 생성하기
	void create(const char * name)
	{
	}

	// 화면에_출력
	void render(float x,float y)
	{
		
	}
};

void main()
{
	붕어빵 붕어A;
	붕어A.버리기();
}

Main::Main() :D3D11Device(800, 600)
{		
	// 사각형 정점 버퍼 생성
	static const float vertices[] =
	{
		-1, 1, 0,	0, 0,
		1, 1, 0,	1, 0,
		-1, -1, 0,	0, 1,
		1, -1, 0,	1, 1
	};
	m_vbQuad = Buffer(D3D11_BIND_VERTEX_BUFFER, vertices);

	// 셰이더 상수 생성
	m_cb_vsCommon = Buffer(D3D11_BIND_CONSTANT_BUFFER, sizeof(CB_VSCommon));
	m_cb_vsBasic = Buffer(D3D11_BIND_CONSTANT_BUFFER, sizeof(CB_VSBasic));
	m_cb_psBasic = Buffer(D3D11_BIND_CONSTANT_BUFFER, sizeof(CB_PSBasic));
	m_cb_vsSkinned = Buffer(D3D11_BIND_CONSTANT_BUFFER, sizeof(CB_VSSkinned));

	// basic 정점 셰이더 & 정점 레이아웃
	static const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, normal), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex, texcoord), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	m_vs_basic = VertexShader(s_vs_basic, layout); // 정점 셰이더

	// skinned 정점 셰이더 & 정점 레이아웃
	static const D3D11_INPUT_ELEMENT_DESC layout_skinned[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, normal), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex, texcoord), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, offsetof(Vertex, blendidx), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(Vertex, blendwgt), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	m_vs_skinned = VertexShader(s_vs_skinned, layout_skinned);

	m_ps_basic = PixelShader(s_ps_basic); // 픽셀 셰이더
	m_ps_noTexture = PixelShader(s_ps_notexture); // 텍스처 없는 픽셀 셰이더
	m_sam = SamplerState(D3D11_TEXTURE_ADDRESS_WRAP, D3D11_FILTER_MIN_MAG_MIP_LINEAR); // 밉맵
	m_depth = DepthStencilState(D3D11_COMPARISON_LESS, D3D11_DEPTH_WRITE_MASK_ALL); // 뎁스 스텐실 스테이트
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

	clear(); // 화면 초기화

	// 텍스처 샘플러 설정
	g_context->PSSetSamplers(0, 1, &m_sam);

	// 깊이 연산 방법 설정
	g_context->OMSetDepthStencilState(m_depth, 0);

	// 셰이더 상수 설정
	g_context->VSSetConstantBuffers(0, 1, &m_cb_vsCommon);
	g_context->PSSetConstantBuffers(0, 1, &m_cb_psBasic);

	// 뷰/프로젝션 행렬 설정
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

	// 프레젠트
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

	// 픽셀 셰이더 설정
	switch (mtl.texCount)
	{
	case 0:
		g_context->PSSetShader(m_ps_noTexture, nullptr, 0);
		break;
	default:
		g_context->PSSetShader(m_ps_basic, nullptr, 0);
		break;
	}

	// 픽셀 셰이더 상수 설정
	CB_PSBasic sc;
	sc.vColor = XMFLOAT4(0.5f, 0.5f, 0.5f, 1);
	g_context->UpdateSubresource(m_cb_psBasic, 0, nullptr, &sc, 0, 0);
}
void Main::setWorld(const aiMatrix4x4 & matrix)
{
	// 버텍스 셰이더 설정
	g_context->VSSetShader(m_vs_basic, nullptr, 0);
	g_context->VSSetConstantBuffers(1, 1, &m_cb_vsBasic);
	g_context->IASetInputLayout(m_vs_basic.getInputLayer());

	CB_VSBasic sc;
	memcpy(&sc.mWorlds, &matrix, sizeof(XMMATRIX));
	g_context->UpdateSubresource(m_cb_vsBasic, 0, nullptr, &sc, 0, 0);
}
void Main::setBoneWorlds(const Matrix4x3 * matrix, size_t m4x3count)
{
	// 버텍스 셰이더 설정
	g_context->VSSetShader(m_vs_skinned, nullptr, 0);
	g_context->VSSetConstantBuffers(1, 1, &m_cb_vsSkinned);
	g_context->IASetInputLayout(m_vs_skinned.getInputLayer());

	CB_VSSkinned sc;
	memcpy(&sc.mWorlds, matrix, m4x3count * (sizeof(float)*4*3));
	g_context->UpdateSubresource(m_cb_vsSkinned, 0, nullptr, &sc, 0, 0);
}
void Main::drawModel(float delta, cbs::Model * model, double * time, aiMatrix4x4 matrix)
{
	// 타이니 렌더링
	{
		//Object a;
		//Object * b;
		// a.function();
		// b->function();

		// 애니메이션 재생
		Model::Pose pose;
		Model::AnimationStatus status;
		status.animation = model->getAnimation(0); // 애니메이션 가져오기
		status.time = *time + delta; // 애니메이션 시간 이동
		pose.set(&status); // 포즈 가져오기
		*time = status.time; // 애니메이션 시간 보정

		// 월드 행렬 설정
		pose.transform(matrix);

		// 렌더링
		render(*model, pose);
	}	
}