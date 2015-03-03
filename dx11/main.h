#pragma once

#include <cbs/d3d11.h>

class Main :public cbs::D3D11Device, public cbs::Assimp, public cbs::ModelRenderer
{
public:
	Main();
	~Main();

	void loop(); // override
	void setMaterial(const cbs::Material & mtl); // override
	void setWorld(const aiMatrix4x4 & matrix); // override

private:

	cbs::Buffer m_vbQuad; // 정점 버퍼
	cbs::Buffer m_constbuffer; // 셰이더 상수 버퍼

	cbs::VertexShader		m_vs; // 정점 셰이더
	cbs::PixelShader		m_ps; // 픽셀 셰이더
	cbs::SamplerState		m_sam; // 텍스처 샘플러
	cbs::DepthStencilState	m_depth; // 뎁스 스텐실 스테이트

	cbs::Model			m_model; // 모델

	DWORD m_startTick;
};
