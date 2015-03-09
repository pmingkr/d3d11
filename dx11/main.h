#pragma once

#define DX11LIB_STATIC_LIBRARY
#include <cbs/d3d11.h>
#include <cbs/deltams.h>

class Main :public cbs::D3D11Device, public cbs::Assimp, public cbs::ModelRenderer
{
public:
	Main(int width, int height);
	~Main();

	virtual void myLoop() = 0;
	void loop(); // override
	void setViewProjection(const XMMATRIX & matrix);
	void setMaterial(const cbs::Material & mtl); // override
	void setWorld(const XMMATRIX & matrix); // override
	void setBoneWorlds(const cbs::Matrix4x3 * matrix, size_t m4x3count); // override

	float getDelta();
	int getWidth();
	int getHeight();

private:

	int						m_width, m_height; // 화면 크기

	cbs::Buffer				m_cb_vsCommon; // 정점 셰이더 공용 상수 버퍼
	cbs::Buffer				m_cb_vsBasic; // 정점 셰이더 상수 버퍼
	cbs::Buffer				m_cb_vsSkinned; // 정점 셰이더 상수 버퍼
	cbs::Buffer				m_cb_psBasic; // 픽셀 셰이더 상수 버퍼

	cbs::VertexShader		m_vs_basic; // 정점 셰이더
	cbs::VertexShader		m_vs_skinned; // 스킨 정점 셰이더
	cbs::PixelShader		m_ps_basic; // 픽셀 셰이더
	cbs::PixelShader		m_ps_noTexture; // 텍스처 없는 픽셀 셰이더
	cbs::SamplerState		m_sam; // 텍스처 샘플러
	cbs::DepthStencilState	m_depth; // 뎁스 스텐실 스테이트

	float					m_delta;
	cbs::DeltaMeasure		m_deltaMeasure; // 델타 측정기
};
