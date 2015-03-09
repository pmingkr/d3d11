#include "main.h"

#include "ps_basic.h"
#include "ps_notexture.h"
#include "vs_basic.h"
#include "vs_skinned.h"

using namespace std;
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

Main::Main(int width, int height) :D3D11Device(width, height, 4)
{		
	m_width = width;
	m_height = height;

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
	
	// 픽셀 셰이더
	m_ps_basic = PixelShader(s_ps_basic);
	m_ps_noTexture = PixelShader(s_ps_notexture);

	// 셰이더 상수 생성
	m_cb_vsCommon = Buffer(D3D11_BIND_CONSTANT_BUFFER, sizeof(CB_VSCommon));
	m_cb_vsBasic = Buffer(D3D11_BIND_CONSTANT_BUFFER, sizeof(CB_VSBasic));
	m_cb_psBasic = Buffer(D3D11_BIND_CONSTANT_BUFFER, sizeof(CB_PSBasic));
	m_cb_vsSkinned = Buffer(D3D11_BIND_CONSTANT_BUFFER, sizeof(CB_VSSkinned));
	
	m_sam = SamplerState(D3D11_TEXTURE_ADDRESS_WRAP, D3D11_FILTER_MIN_MAG_MIP_LINEAR); // 밉맵
	m_depth = DepthStencilState(D3D11_COMPARISON_LESS, D3D11_DEPTH_WRITE_MASK_ALL); // 뎁스 스텐실 스테이트

}
Main::~Main()
{
}

void Main::onKeyDown(int key, bool repeat)
{
}
void Main::onKeyUp(int key)
{
}

void Main::procedure(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		onKeyDown(wParam, (HIWORD(lParam) & KF_REPEAT) != 0);
		break;
	case WM_KEYUP:
		onKeyUp(wParam);
		break;
	}
}
void Main::loop()
{
	m_delta = (float)m_deltaMeasure.measureDelta(); // 시간 변화량 측정

	clear(); // 화면 초기화

	// 텍스처 샘플러 설정
	g_context->PSSetSamplers(0, 1, &m_sam);

	// 깊이 연산 방법 설정
	g_context->OMSetDepthStencilState(m_depth, 0);

	// 셰이더 상수 설정
	g_context->VSSetConstantBuffers(0, 1, &m_cb_vsCommon);
	g_context->PSSetConstantBuffers(0, 1, &m_cb_psBasic);

	myLoop();

	// 결과 출력
	g_chain->Present(1, 0);
}

void Main::setViewProjection(const XMMATRIX & matrix)
{
	CB_VSCommon sc;
	sc.mVP = XMMatrixTranspose(matrix);
	g_context->UpdateSubresource(m_cb_vsCommon, 0, nullptr, &sc, 0, 0);
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
void Main::setWorld(const XMMATRIX & matrix)
{
	// 버텍스 셰이더 설정
	g_context->VSSetShader(m_vs_basic, nullptr, 0);
	g_context->VSSetConstantBuffers(1, 1, &m_cb_vsBasic);
	g_context->IASetInputLayout(m_vs_basic.getInputLayer());

	CB_VSBasic sc;
	sc.mWorlds = matrix;
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

float Main::getDelta()
{
	return m_delta;
}
int Main::getWidth()
{
	return m_width;
}
int Main::getHeight()
{
	return m_height;
}

#pragma region 메모리 누수 감지

#pragma warning(disable:4074)
#pragma init_seg(compiler)

struct __StaticRun
{
	__StaticRun()
	{
		//_crtBreakAlloc = 252;
	}
	~__StaticRun()
	{
		_CrtDumpMemoryLeaks(); // 메모리 누수 감지
	}
} __staticRun;

#pragma endregion
